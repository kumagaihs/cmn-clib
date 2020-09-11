#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include <string>
#include <vector>

#include "cmn-tools/CommandException.hpp"

namespace cmnclib {
namespace tools {
namespace command {

/**
 * コマンドの基底クラス
 */
class Command {
private:
	static std::vector<Command*> m_allCommmands;

public:
	/**
	 * コマンド名を取得する
	 * @return コマンド名
	 */
	virtual std::string name() { return ""; }

	/**
	 * コマンドの概要を取得する
	 * @return コマンドの概要
	 */
	virtual std::string summary() { return ""; }

	/**
	 * コマンドヘルプを取得する
	 * @return コマンドヘルプ
	 */
	virtual std::string help() { return ""; }

	/**
	 * コマンドを実行する
	 * @param args コマンド引数
	 * @return コマンドの実行結果（リターンコード）
	 */
	virtual int execute(std::vector<std::string> args) { return 0; }

	/**
	 * cmn-toolsに登録されている全てのクラスを取得する
	 * @return 全Commandインスタンス
	 */
	static std::vector<Command*>& getAllCommands();

	/**
	 * コマンド名nameのコマンドインスタンスを取得する。
	 * @param name コマンド名
	 * @return Commandインスタンス
	 */
	static Command* getCommand(const std::string& name);

	/**
	 * コマンドを新たに登録する。
	 * @param command 追加登録するコマンド
	 */
	static void registCommand(Command *command);
};





}
}
}

#endif
