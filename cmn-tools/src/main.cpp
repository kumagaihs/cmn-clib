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
//  コマンド列挙　　※新規コマンド実装時はここに追加すること。
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

	// サブコマンド名の取得
	if (argc <= 1) {
		// サブコマンドが指定されていない場合はヘルプを表示
		commandName = "help";
	}
	else {
		commandName = argv[1];
	}
	// サブコマンドのオプションを取得
	for (int i = 2; i < argc; i++) {
		args.push_back(argv[i]);
	}

	// コマンド初期化
	setupCommand();

	// コマンド実行
	try {
		cmd::Command *command = cmd::Command::getCommand(commandName);
		return command->execute(args);
	}
	catch (cmd::CommandException e) {
		std::cout << e.getMessage();
		return 1;
	}
}
