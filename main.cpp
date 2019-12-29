#include <iostream>
#include <fstream>
#include "serializable.hpp"


struct gameobject : Serializable {
	sStr title = {"Mario"};

  sFields
	__(title)
	sEnd
};

struct scene : Serializable {
	sInt width = 800;
	sInt height = 600;
	gameobject mario;

	sFields
	__(width)
	__(height)
	__(mario)
	sEnd
};


int main()
{
    std::cout << "Hi!\n";
    scene data;
    std::cout << data._fields_;

    return 0;
}
