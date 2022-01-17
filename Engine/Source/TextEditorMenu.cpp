#include "TextEditorMenu.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "FileSystem.h"
#include "Globals.h"


#include "Imgui/imgui.h"
#include <iostream>
#include <thread>
#include <fstream>

#include <GL\glew.h>

#include "Profiling.h"

TextEditorMenu::TextEditorMenu() : Menu(true)
{
}

TextEditorMenu::~TextEditorMenu()
{
}


bool TextEditorMenu::Start()
{
	auto language = TextEditor::LanguageDefinition::CPlusPlus();
	textEditor.SetLanguageDefinition(language);

	SetFile("Assets/Scripts/Tank.cpp");

	return true;
}

bool TextEditorMenu::Update(float dt)
{
	ImGui::Begin("Text Editor", &active, ImGuiWindowFlags_MenuBar);

	auto pos = textEditor.GetCursorPosition();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Open")) {
				if (allScriptsFiles.empty())
					GetAllFiles();

				std::vector<std::string>::iterator item = allScriptsFiles.begin();
				for (; item != allScriptsFiles.end(); ++item) {
					std::string extension;
					app->fs->SplitFilePath((*item).data(), nullptr, nullptr, &extension);
					if (app->StringCmp(extension.data(), "cpp") || app->StringCmp(extension.data(), "h")) {
						if (ImGui::MenuItem((*item).data(), nullptr, nullptr, !app->StringCmp((*item).data(), fileName.data()))) {
							SetFile(std::string(SCRIPTS_FOLDER + (*item)).data());
						}
					}
				}
				ImGui::EndMenu();
			}
			else if (!allScriptsFiles.empty())
				allScriptsFiles.clear();

			if (ImGui::MenuItem("Save"))
			{
				std::string toSave = textEditor.GetText();
				app->fs->Save(fileToEdit.data(), toSave.data(), toSave.size());
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool read = textEditor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &read))
				textEditor.SetReadOnly(read);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !read && textEditor.CanUndo()))
				textEditor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !read && textEditor.CanRedo()))
				textEditor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, textEditor.HasSelection()))
				textEditor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !read && textEditor.HasSelection()))
				textEditor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !read && textEditor.HasSelection()))
				textEditor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !read && ImGui::GetClipboardText() != nullptr))
				textEditor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				textEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(textEditor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				textEditor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				textEditor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				textEditor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", pos.mLine + 1, pos.mColumn + 1, textEditor.GetTotalLines(),
		textEditor.IsOverwrite() ? "Ovr" : "Ins",
		textEditor.CanUndo() ? "*" : " ",
		textEditor.GetLanguageDefinition().mName.c_str(), fileName.data());

	textEditor.Render("Text Editor");

	isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows);

	ImGui::End();

	return true;
}

void TextEditorMenu::SetFile(const char* file)
{
	fileToEdit = std::string(file);
	fileName = app->fs->GetBaseFileNameWithExtension(file);

	std::ifstream t(file);
	if (t.good()) {
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		textEditor.SetText(str);
	}
}

bool TextEditorMenu::IsFocused() const
{
	if (active && isFocused) {
		return true;
	}
	return false;
}

void TextEditorMenu::GetAllFiles()
{
	app->fs->DiscoverFiles(SCRIPTS_FOLDER, allScriptsFiles);
}
