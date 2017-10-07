package baci.program;

import baci.interpreter.*;
/**
 * InstructionWriteln
 * @author: David Strite
 */
public class InstructionWriteln extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionWriteln(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	String string = "\n";
	interpreter.getConsole().print(string);
	interpreter.getCurrentProcess().getConsole().print(string);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "WRITELN";
}
}
