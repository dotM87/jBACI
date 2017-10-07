package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionExitProc
 * @author: David Strite
 */
public class InstructionExitProc extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionExitProc(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if invalid pcode address is encountered
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	Program program = getProgram();

	int bottom = currProc.getBottom();
	int identifierIndex = stack.getInt(bottom+4);
	if(program.getIdentifier(identifierIndex).isAtomic()) {
		int atomicCount = currProc.getAtomicCount();
		currProc.setAtomicCount(atomicCount-1);
	}
	currProc.setTop(bottom-1);
	int pcodeAddress=stack.getInt(bottom+1);
	program.validatePcodeAddress(pcodeAddress);
	currProc.setCurrentPcode(pcodeAddress);

	if(currProc.getCurrentPcode()!=0) {
		Address address = stack.getAddress(bottom+3);
		if(address.getProcess().getStack().getInt(address.getIndex()+1)==-1) {
			address = address.getProcess().getStack().getAddress(address.getIndex()+3);
		}
		currProc.setBottom(address.getIndex());
	} else {
		interpreter.stopProcess(currProc.getIndex());
	}
	program.buildVariables(currProc.getVariableModel(),program.getPcode(currProc.getCurrentPcode()).getBlockIndex(),currProc.getBottom());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "EXIT_PROC";
}
}
