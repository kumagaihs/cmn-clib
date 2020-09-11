#ifndef _COMMAND_EXCEPTION_HPP
#define _COMMAND_EXCEPTION_HPP

#include <exception>

namespace cmnclib {
namespace tools {
namespace command {

class CommandException : public std::exception {
private:
	std::string m_command;
	std::string m_message;
	std::exception* m_originalError;
	std::string m_file;
	int m_line;

public:
	CommandException() : CommandException("", "", 0, "", nullptr) {};
	CommandException(std::string command, std::string message) : CommandException(command, "", 0, message, nullptr) {}
	CommandException(std::string command, std::string message, std::exception* e) : CommandException(command, "", 0, message, e) {}
	CommandException(std::string command, std::string file, int line, std::string message) : CommandException(command, file, line, message, nullptr) {}
	CommandException(std::string command, std::string file, int line, std::string message, std::exception* e) : m_command(command), m_file(file), m_line(line), m_message(message), m_originalError(e) {}

	std::string getMessage() {
		return m_command + " : " + m_message + "\n"
				+ " at " + m_file + ":" + std::to_string(m_line) + "\n"
				+ (m_originalError == nullptr ? "" : m_originalError->what());
	}
};

}
}
}

#endif
