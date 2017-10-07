package baci.program;

import baci.interpreter.*;
/**
 * InstructionHalt
 * @author: David Strite
 */
public class InstructionHalt extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionHalt(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	interpreter.setStatus(Interpreter.STATUS_FINISH);
	interpreter.stopProcess(0);
	interpreter.openHistoryFile(null);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "HALT";
}
}
