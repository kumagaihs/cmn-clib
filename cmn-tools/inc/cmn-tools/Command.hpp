#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include <string>
#include <vector>

#include "cmn-tools/CommandException.hpp"

namespace cmnclib {
namespace tools {
namespace command {

/**
 * �R�}���h�̊��N���X
 */
class Command {
private:
	static std::vector<Command*> m_allCommmands;

public:
	/**
	 * �R�}���h�����擾����
	 * @return �R�}���h��
	 */
	virtual std::string name() { return ""; }

	/**
	 * �R�}���h�̊T�v���擾����
	 * @return �R�}���h�̊T�v
	 */
	virtual std::string summary() { return ""; }

	/**
	 * �R�}���h�w���v���擾����
	 * @return �R�}���h�w���v
	 */
	virtual std::string help() { return ""; }

	/**
	 * �R�}���h�����s����
	 * @param args �R�}���h����
	 * @return �R�}���h�̎��s���ʁi���^�[���R�[�h�j
	 */
	virtual int execute(std::vector<std::string> args) { return 0; }

	/**
	 * cmn-tools�ɓo�^����Ă���S�ẴN���X���擾����
	 * @return �SCommand�C���X�^���X
	 */
	static std::vector<Command*>& getAllCommands();

	/**
	 * �R�}���h��name�̃R�}���h�C���X�^���X���擾����B
	 * @param name �R�}���h��
	 * @return Command�C���X�^���X
	 */
	static Command* getCommand(const std::string& name);

	/**
	 * �R�}���h��V���ɓo�^����B
	 * @param command �ǉ��o�^����R�}���h
	 */
	static void registCommand(Command *command);
};





}
}
}

#endif
