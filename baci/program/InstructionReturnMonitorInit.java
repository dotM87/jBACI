package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionReturnMonitorInit
 * @author: David Strite
 */
public class InstructionReturnMonitorInit extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionReturnMonitorInit(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if pcode address is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	int pcodeAddress=stack.getInt(currProc.getBottom()+1);
	getProgram().validatePcodeAddress(pcodeAddress);
	currProc.setCurrentPcode(pcodeAddress);
	stack.set(currProc.getBottom()+1,-1);
	currProc.setBottom(stack.getAddress(currProc.getBottom()+2).getIndex());
	currProc.getDisplay().set(1,new Address(interpreter.getProcess(0),currProc.getBottom()));
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "RET_MONINIT";
}
}
