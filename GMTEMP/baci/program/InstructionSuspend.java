package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionSuspend
 * @author: David Strite
 */
public class InstructionSuspend extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionSuspend(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the suspended process is in an atomic function/procedure
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	if(currProc.getAtomicCount()!=0) {
		throw new BaciException("Suspended thread is in a atomic function/procedure.");
	}
	currProc.setActive(false);
	currProc.setSuspend(null);
	interpreter.setNumInstBeforeSwap(0);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SUSPEND current process";
}
}
