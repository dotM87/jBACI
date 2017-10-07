package baci.program;

import baci.interpreter.*;
/**
 * InstructionRandom
 * @author: David Strite
 */
public class InstructionRandom extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionRandom(Program program, int x, int y) {
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
	int max = stack.getInt(top);
	stack.set(top,interpreter.getRandomInt(max));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "RANDOM, s[t]=random(0..s[oldt]-1)";
}
}
