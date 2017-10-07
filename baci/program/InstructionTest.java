package baci.program;

import baci.interpreter.*;
/**
 * InstructionTest is the super class of the Test* instructions which are comparisons
 * @author: David Strite
 */
abstract public class InstructionTest extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionTest(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()-1;
	currProc.setTop(top);
	Stack stack = currProc.getStack();
	stack.set(top,doTest(stack.getInt(top),stack.getInt(top+1))?1:0);
}
/**
 * Insert the method's description here.
 * Creation date: (9/11/2001 8:39:23 PM)
 * @return boolean
 * @param arg1 int
 * @param arg2 int
 */
protected abstract boolean doTest(int arg1, int arg2);
}
