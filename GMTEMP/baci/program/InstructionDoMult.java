package baci.program;

import baci.interpreter.*;
/**
 * InstructionDoMult
 * @author: David Strite
 */
public class InstructionDoMult extends InstructionDo {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionDoMult(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Insert the method's description here.
 * Creation date: (9/11/2001 8:54:47 PM)
 * @return int
 * @param arg1 int
 * @param arg2 int
 */
protected int doOp(int arg1, int arg2) {
	return arg1*arg2;
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "DO_MUL, pop(1), s[t]=(s[oldt-1]*s[oldt])";
}
}
