package baci.program;

import baci.interpreter.*;
/**
 * InstructionCopyString
 * @author: David Strite
 */
public class InstructionCopyString extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCopyString(Program program, int x, int y) {
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

	Address toAddress=stack.getAddress(top-1);
	Address fromAddress=stack.getAddress(top);
	toAddress.getProcess().getStack().set(toAddress.getIndex(),fromAddress.getProcess().getStack().get(fromAddress.getIndex()));
	top-=2;
	currProc.setTop(top);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "COPY_STRING from &s[s[t]] to s[s[t-1]], pop(2)";
}
}
