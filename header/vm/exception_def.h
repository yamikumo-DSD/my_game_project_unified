//exception_def.h

#pragma once

#include <exception>

namespace MyVM
{
	class Exception : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
		virtual ~Exception(void) {}
	};

	class StackOverFlow : public Exception
	{
	public:
		using Exception::Exception;
		~StackOverFlow(void) {}
	};

	class StackUnderFlow : public Exception
	{
	public:
		using Exception::Exception;
		~StackUnderFlow(void) {}
	};

	class InvalidInstruction : public Exception
	{
	public:
		using Exception::Exception;
		~InvalidInstruction(void) {}
	};

	class ByteCodeOverRun : public Exception
	{
	public:
		using Exception::Exception;
		~ByteCodeOverRun(void) {}
	};
}