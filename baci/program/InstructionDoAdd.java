package baci.program;

import baci.interpreter.*;
/**
 * InstructionDoAdd
 * @author: David Strite
 */
public class InstructionDoAdd extends InstructionDo {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionDoAdd(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs instruction's operation on the two arguments
 * @param arg1 int
 * @param arg2 int
 */
protected int doOp(int arg1, int arg2) {
	return arg1+arg2;
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "DO_ADD, pop(1), s[t]=(s[oldt-1]+s[oldt])";
}
}
