#include "Editor.h"

int main(int argc, char** agrv)
{
	Alexio::Editor* editor = new Alexio::Editor();
	editor->Run();
	delete editor;
	return 0;
}