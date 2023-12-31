// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkTextMapper.h"

#include "vtkActor2D.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkOverrideInformation.h"
#include "vtkOverrideInformationCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTextProperty.h"

//------------------------------------------------------------------------------
int TestFreeTypeTextMapper(int argc, char* argv[])
{
  if (argc < 2)
  {
    cerr << "Missing font filename." << endl;
    return EXIT_FAILURE;
  }

  std::string uncodeFontFile(argv[1]);
  std::string str = "Sample multiline\ntext rendered\nusing FreeTypeTools.";

  vtkNew<vtkTextMapper> mapper1;
  vtkNew<vtkActor2D> actor1;
  actor1->SetMapper(mapper1);
  mapper1->GetTextProperty()->SetFontSize(20);
  mapper1->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
  mapper1->GetTextProperty()->SetJustificationToLeft();
  mapper1->GetTextProperty()->SetVerticalJustificationToTop();
  mapper1->GetTextProperty()->SetFontFamilyToTimes();
  mapper1->SetInput(str.c_str());
  actor1->SetPosition(10, 590);

  vtkNew<vtkTextMapper> mapper2;
  vtkNew<vtkActor2D> actor2;
  actor2->SetMapper(mapper2);
  mapper2->GetTextProperty()->SetFontSize(20);
  mapper2->GetTextProperty()->SetColor(0.0, 1.0, 0.0);
  mapper2->GetTextProperty()->SetJustificationToRight();
  mapper2->GetTextProperty()->SetVerticalJustificationToTop();
  mapper2->GetTextProperty()->SetFontFamilyToCourier();
  mapper2->SetInput(str.c_str());
  actor2->SetPosition(590, 590);

  vtkNew<vtkTextMapper> mapper3;
  vtkNew<vtkActor2D> actor3;
  actor3->SetMapper(mapper3);
  mapper3->GetTextProperty()->SetFontSize(20);
  mapper3->GetTextProperty()->SetColor(0.0, 0.0, 1.0);
  mapper3->GetTextProperty()->SetJustificationToLeft();
  mapper3->GetTextProperty()->SetVerticalJustificationToBottom();
  mapper3->GetTextProperty()->SetItalic(1);
  mapper3->SetInput(str.c_str());
  actor3->SetPosition(10, 10);

  vtkNew<vtkTextMapper> mapper4;
  vtkNew<vtkActor2D> actor4;
  actor4->SetMapper(mapper4);
  mapper4->GetTextProperty()->SetFontSize(20);
  mapper4->GetTextProperty()->SetColor(0.3, 0.4, 0.5);
  mapper4->GetTextProperty()->SetJustificationToRight();
  mapper4->GetTextProperty()->SetVerticalJustificationToBottom();
  mapper4->GetTextProperty()->SetBold(1);
  mapper4->GetTextProperty()->SetShadow(1);
  mapper4->GetTextProperty()->SetShadowOffset(-3, 2);
  mapper4->SetInput(str.c_str());
  actor4->SetPosition(590, 10);

  vtkNew<vtkTextMapper> mapper5;
  vtkNew<vtkActor2D> actor5;
  actor5->SetMapper(mapper5);
  mapper5->GetTextProperty()->SetFontSize(20);
  mapper5->GetTextProperty()->SetColor(1.0, 1.0, 0.0);
  mapper5->GetTextProperty()->SetJustificationToCentered();
  mapper5->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper5->GetTextProperty()->SetBold(1);
  mapper5->GetTextProperty()->SetItalic(1);
  mapper5->GetTextProperty()->SetShadow(1);
  mapper5->GetTextProperty()->SetShadowOffset(5, -8);
  mapper5->SetInput(str.c_str());
  actor5->SetPosition(300, 300);

  vtkNew<vtkTextMapper> mapper6;
  vtkNew<vtkActor2D> actor6;
  actor6->SetMapper(mapper6);
  mapper6->GetTextProperty()->SetFontSize(16);
  mapper6->GetTextProperty()->SetColor(1.0, 0.5, 0.2);
  mapper6->GetTextProperty()->SetJustificationToCentered();
  mapper6->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper6->GetTextProperty()->SetOrientation(45);
  mapper6->SetInput(str.c_str());
  actor6->SetPosition(300, 450);

  vtkNew<vtkTextMapper> mapper7;
  vtkNew<vtkActor2D> actor7;
  actor7->SetMapper(mapper7);
  mapper7->GetTextProperty()->SetFontSize(16);
  mapper7->GetTextProperty()->SetColor(0.5, 0.2, 1.0);
  mapper7->GetTextProperty()->SetJustificationToLeft();
  mapper7->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper7->GetTextProperty()->SetOrientation(45);
  mapper7->SetInput(str.c_str());
  actor7->SetPosition(100, 200);

  vtkNew<vtkTextMapper> mapper8;
  vtkNew<vtkActor2D> actor8;
  actor8->SetMapper(mapper8);
  mapper8->GetTextProperty()->SetFontSize(16);
  mapper8->GetTextProperty()->SetColor(0.8, 1.0, 0.3);
  mapper8->GetTextProperty()->SetJustificationToRight();
  mapper8->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper8->GetTextProperty()->SetOrientation(45);
  mapper8->SetInput(str.c_str());
  actor8->SetPosition(500, 200);

  // Mathtext tests

  // Test that escaped "$" are passed through to freetype:
  vtkNew<vtkTextMapper> mapper9;
  vtkNew<vtkActor2D> actor9;
  actor9->SetMapper(mapper9);
  mapper9->GetTextProperty()->SetFontSize(12);
  mapper9->GetTextProperty()->SetColor(0.2, 0.5, 1.0);
  mapper9->SetInput("Escaped dollar signs:\n\\$10, \\$20");
  actor9->SetPosition(100, 450);

  vtkNew<vtkTextMapper> mapper10;
  vtkNew<vtkActor2D> actor10;
  actor10->SetMapper(mapper10);
  mapper10->GetTextProperty()->SetFontSize(16);
  mapper10->GetTextProperty()->SetColor(0.5, 0.2, 1.0);
  mapper10->GetTextProperty()->SetJustificationToRight();
  mapper10->GetTextProperty()->SetOrientation(45);
  mapper10->SetInput("Test MathText $\\int_0^\\infty\\frac{2\\pi}"
                     "{x - \\frac{z}{4}}\\,dx$");
  actor10->SetPosition(590, 300);

  // Invalid latex markup -- should fallback to freetype.
  vtkNew<vtkTextMapper> mapper11;
  vtkNew<vtkActor2D> actor11;
  actor11->SetMapper(mapper11);
  mapper11->GetTextProperty()->SetFontSize(15);
  mapper11->GetTextProperty()->SetColor(1.0, 0.5, 0.2);
  mapper11->SetInput("Test FreeType fallback:\n$\\asdf$");
  actor11->SetPosition(10, 350);

  // Both $...$ and \\$
  vtkNew<vtkTextMapper> mapper12;
  vtkNew<vtkActor2D> actor12;
  actor12->SetMapper(mapper12);
  mapper12->GetTextProperty()->SetFontSize(18);
  mapper12->GetTextProperty()->SetColor(0.0, 1.0, 0.7);
  mapper12->SetInput("Test MathText '\\$' $\\$\\sqrt[3]{8}$");
  actor12->SetPosition(10, 300);

  // $...$ without other text.
  vtkNew<vtkTextMapper> mapper13;
  vtkNew<vtkActor2D> actor13;
  actor13->SetMapper(mapper13);
  mapper13->GetTextProperty()->SetFontSize(18);
  mapper13->GetTextProperty()->SetColor(0.2, 1.0, 1.0);
  mapper13->SetInput("$A = \\pi r^2$");
  actor13->SetPosition(10, 250);

  // Numbers, using courier, Text that gets 'cut off'
  vtkNew<vtkTextMapper> mapper14;
  vtkNew<vtkActor2D> actor14;
  actor14->SetMapper(mapper14);
  mapper14->GetTextProperty()->SetFontSize(21);
  mapper14->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
  mapper14->GetTextProperty()->SetJustificationToCentered();
  mapper14->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper14->GetTextProperty()->SetBold(1);
  mapper14->GetTextProperty()->SetItalic(1);
  mapper14->GetTextProperty()->SetFontFamilyToCourier();
  mapper14->SetInput("4.0");
  actor14->SetPosition(500, 400);

  // UTF-8 freetype handling:
  vtkNew<vtkTextMapper> mapper15;
  vtkNew<vtkActor2D> actor15;
  actor15->SetMapper(mapper15);
  mapper15->GetTextProperty()->SetFontFile(uncodeFontFile.c_str());
  mapper15->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
  mapper15->GetTextProperty()->SetJustificationToCentered();
  mapper15->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper15->GetTextProperty()->SetFontSize(18);
  mapper15->GetTextProperty()->SetColor(0.0, 1.0, 0.7);
  mapper15->SetInput("UTF-8 FreeType: \xce\xa8\xd2\x94\xd2\x96\xd1\x84\xd2\xbe");
  actor15->SetPosition(300, 110);

  // Test for rotated kerning (PR#15301)
  vtkNew<vtkTextMapper> mapper16;
  vtkNew<vtkActor2D> actor16;
  actor16->SetMapper(mapper16);
  mapper16->GetTextProperty()->SetFontFile(uncodeFontFile.c_str());
  mapper16->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
  mapper16->GetTextProperty()->SetJustificationToCentered();
  mapper16->GetTextProperty()->SetVerticalJustificationToCentered();
  mapper16->GetTextProperty()->SetFontSize(18);
  mapper16->GetTextProperty()->SetOrientation(90);
  mapper16->GetTextProperty()->SetColor(0.0, 1.0, 0.7);
  mapper16->SetInput("oTeVaVoVAW");
  actor16->SetPosition(300, 200);

  // Boring rendering setup....

  vtkNew<vtkRenderer> ren;
  ren->SetBackground(0.1, 0.1, 0.1);
  vtkNew<vtkRenderWindow> win;
  win->SetSize(600, 600);
  win->AddRenderer(ren);
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win);

  ren->AddActor(actor1);
  ren->AddActor(actor2);
  ren->AddActor(actor3);
  ren->AddActor(actor4);
  ren->AddActor(actor5);
  ren->AddActor(actor6);
  ren->AddActor(actor7);
  ren->AddActor(actor8);
  ren->AddActor(actor9);
  ren->AddActor(actor10);
  ren->AddActor(actor11);
  ren->AddActor(actor12);
  ren->AddActor(actor13);
  ren->AddActor(actor14);
  ren->AddActor(actor15);
  ren->AddActor(actor16);

  win->SetMultiSamples(0);
  win->Render();
  win->GetInteractor()->Initialize();
  win->GetInteractor()->Start();

  return EXIT_SUCCESS;
}
