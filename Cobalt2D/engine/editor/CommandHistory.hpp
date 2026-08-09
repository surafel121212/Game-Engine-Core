#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace cobalt {

class EditorCommand {
public:
  virtual ~EditorCommand() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
  virtual std::string name() const = 0;
};

class LambdaCommand final : public EditorCommand {
public:
  LambdaCommand(std::string commandName, std::function<void()> execute,
                std::function<void()> undo)
      : name_(std::move(commandName)), execute_(std::move(execute)), undo_(std::move(undo)) {}
  void execute() override { execute_(); }
  void undo() override { undo_(); }
  std::string name() const override { return name_; }

private:
  std::string name_;
  std::function<void()> execute_;
  std::function<void()> undo_;
};

class CommandHistory {
public:
  void execute(std::unique_ptr<EditorCommand> command);
  bool undo();
  bool redo();
  void clear();
  bool canUndo() const { return cursor_ > 0; }
  bool canRedo() const { return cursor_ < commands_.size(); }
  std::string lastCommand() const;

private:
  std::vector<std::unique_ptr<EditorCommand>> commands_;
  std::size_t cursor_{0};
};

}  // namespace cobalt