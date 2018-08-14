#pragma once

#include <vector>
#include <string>

#include "Yggdrasil.h"
#include "AudioDataflow.h"
#include "Block.h"

class Object { };

class AudioObject : public Object
{
protected:
	std::vector<Block>      read_block();
	void                    write_block(Block b, uint output);

	std::string             args;
	
public:
	std::vector<AudioInput> inputs;
	std::vector<AudioOutput>outputs;
	
	std::vector<float>      data;

	virtual void            run() = 0;
	virtual void            finish();

	AudioObject();
};