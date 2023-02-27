#include "aio_pch.h"
#include "Engine.h"

int main(int argc, char** agrv)
{
	Alexio::Ref<Alexio::Engine> engine = Create();
	engine->Run();
	return 0;
}