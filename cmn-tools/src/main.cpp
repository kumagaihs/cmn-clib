#include <iostream>
#include <vector>

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"

#include "cmn-tools/commands/HelpCommand.hpp"
#include "cmn-tools/commands/GetclipCommand.hpp"
#include "cmn-tools/commands/SetclipCommand.hpp"
#include "cmn-tools/commands/GrepCommand.hpp"
#include "cmn-tools/commands/Win32SkeletonCommand.hpp"

namespace cmd = cmnclib::tools::command;

//################################################
//  �R�}���h�񋓁@�@���V�K�R�}���h�������͂����ɒǉ����邱�ƁB
//################################################
cmd::HelpCommand cmd_help;
cmd::GetclipCommand cmd_getclip;
cmd::SetclipCommand cmd_setclip;
cmd::GrepCommand cmd_grep;
cmd::Win32SkeletonCommand cmd_win32skeleton;

void setupCommand() {
	cmd::Command::registCommand(&cmd_help);
	cmd::Command::registCommand(&cmd_getclip);
	cmd::Command::registCommand(&cmd_setclip);
	cmd::Command::registCommand(&cmd_grep);
	cmd::Command::registCommand(&cmd_win32skeleton);
}
//################################################


int main(int argc, char *argv[]) {

	std::string commandName;
	std::vector<std::string> args;

	// �T�u�R�}���h���̎擾
	if (argc <= 1) {
		// �T�u�R�}���h���w�肳��Ă��Ȃ��ꍇ�̓w���v��\��
		commandName = "help";
	}
	else {
		commandName = argv[1];
	}
	// �T�u�R�}���h�̃I�v�V�������擾
	for (int i = 2; i < argc; i++) {
		args.push_back(argv[i]);
	}

	// �R�}���h������
	setupCommand();

	// �R�}���h���s
	try {
		cmd::Command *command = cmd::Command::getCommand(commandName);
		return command->execute(args);
	}
	catch (cmd::CommandException e) {
		std::cout << e.getMessage();
		return 1;
	}
}
