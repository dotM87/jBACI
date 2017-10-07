package baci.program;

import baci.interpreter.*;
/**
 * InstructionTestEqualKeep
 * @author: David Strite
 */
public class InstructionTestEqualKeep extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionTestEqualKeep(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop();
	Stack stack = currProc.getStack();
	stack.set(top,stack.getInt(top-1)==stack.getInt(top)?1:0);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "TEST_EQ_KEEP, s[t]=(s[t-1]==s[t])";
}
}
