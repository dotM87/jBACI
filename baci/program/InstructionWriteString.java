package baci.program;

import baci.interpreter.*;
/**
 * InstructionWriteString
 * @author: David Strite
 */
public class InstructionWriteString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWriteString(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();
	Address address = stack.getAddress(top);
	String string = address.getProcess().getStack().getString(address.getIndex());
	interpreter.getConsole().print(string);
	currProc.getConsole().print(string);
	currProc.setTop(top-1);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WRITE_STRING at s[s[t]] to stdout, pop(1)";
}
}
