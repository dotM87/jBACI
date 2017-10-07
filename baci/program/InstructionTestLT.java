package baci.program;

import baci.interpreter.*;
/**
 * InstructionTestLT
 * @author: David Strite
 */
public class InstructionTestLT extends InstructionTest {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionTestLT(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Insert the method's description here.
 * Creation date: (9/11/2001 8:39:23 PM)
 * @return boolean
 * @param arg1 int
 * @param arg2 int
 */
protected boolean doTest(int arg1, int arg2) {
	return arg1 < arg2;
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "TEST_LT, pop(1), s[t]=(s[oldt-1]<s[oldt])";
}
}
