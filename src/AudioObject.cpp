
#include <vector>
#include <string>
#include <iostream>

#include "AudioObject.h"
#include "StringFormat.h"

namespace Yggdrasil {

void AudioObject::finish() { }

void AudioObject::implement()
{
	for (uint n = 0; n < inputs.size(); n++)
	{
		float value = inputs[n].read_value();
		in[n][0] = value;
		in[n].increment_pointer();
	}

	for (auto const& value : linked_values)
		if (inputs[value.input].is_connected())
			*value.parameter = in[value.input][0];
	run(in, out);

	for (uint n = 0; n < outputs.size(); n++)
	{
		float value = out[n][0];
		outputs[n].write_value(value);
		out[n].increment_pointer();
	}
}

bool AudioObject::is_connected(uint in)
{
	return inputs[in].is_connected();
}

void AudioObject::init(int num_inputs, int num_outputs,
                       std::vector<std::string> args, std::vector<float*> float_members)
{
	set_io(num_inputs, num_outputs);
	get_float_args(args, float_members);
}

void AudioObject::get_float_args(std::vector<std::string> args, std::vector<float*> float_members)
{

	for (uint n = 0; n < float_members.size() && n < args.size(); n++)
	{     
   		try         { *float_members[n] = float(std::stof(args[n], nullptr)); }
		catch (const std::invalid_argument&) { log("Failed to initialize object parameter"); }
	}
}

void AudioObject::set_io(int num_inputs, int num_outputs)
{
	outputs.resize(num_outputs);
	inputs.resize(num_inputs);

	out.resize(num_outputs);
	in.resize(num_inputs);
}

void AudioObject::request_buffer_size(int count)
{
	for (auto& buffer : in) buffer.resize_stream(count);
	for (auto& buffer : out) buffer.resize_stream(count);
	buffer_size = count;
}

void AudioObject::set_defval(float* parameter, float default_value, int input)
{
	linked_values.push_back(linked_value(parameter, default_value, input));
}

bool AudioObject::is_gate_high(uint input)
{
	if (in[input][0] >= AudioObject::gate_threshold) return true;
	return false;
}

bool AudioObject::gate_opened(uint input)
{
	if (in[input][0] >= AudioObject::gate_threshold &&
		in[input][-1] < AudioObject::gate_threshold) return true;
	return false;
}

bool AudioObject::gate_closed(uint input)
{
	if (in[input][0] <   AudioObject::gate_threshold &&
		in[input][-1] >= AudioObject::gate_threshold) return true;
	return false;
}

}
