package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionCall
 * @author: David Strite
 */
public class InstructionCall extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCall(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if invalid pcode address is called
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	Program program = getProgram();
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	
	currProc.setActive(true);
	int calleeAR0 = currProc.getTop()-getY();
	int identifierIndex = stack.getInt(calleeAR0+4);
	int level = program.getIdentifier(identifierIndex).getLevel();
	int top = stack.getInt(calleeAR0+3)+calleeAR0;

	//update display	
	currProc.getDisplay().set(level+1,new Address(currProc,calleeAR0));
	//store return address
	stack.set(calleeAR0+1,currProc.getCurrentPcode());
	Identifier identifier = program.getIdentifier(identifierIndex);
	if(getX()==-1) {
		//monitor proc call
		int monitorAR = program.getIdentifier(identifier.getMon()).getMon();
		currProc.getDisplay().set(level,new Address(interpreter.getProcess(0),monitorAR));
		stack.set(calleeAR0+2,new Address(currProc,monitorAR));
	} else {
		Address address = currProc.getDisplay().getAddress(level);
		stack.set(calleeAR0+2,new Address(address.getProcess(),address.getIndex()));
	}
	if(interpreter.isInCobeginBlock()) {
		stack.set(calleeAR0+3,new Address(currProc,interpreter.getProcess(0).getBottom()));
		currProc.setIdentifierIndex(identifierIndex);
	} else {
		stack.set(calleeAR0+3,new Address(currProc,currProc.getBottom()));
	}
	currProc.setBottom(calleeAR0);
	currProc.setTop(top);
	//Jump to pcode address
	program.validatePcodeAddress(identifier.getAdr());
	currProc.setCurrentPcode(identifier.getAdr());
	if(identifier.isAtomic()) {
		//this is an atomic call, increment atomic count
		currProc.setAtomicCount(currProc.getAtomicCount()+1);
	}
	//build new list of variables for this block
	program.buildVariables(currProc.getVariableModel(),program.getPcode(currProc.getCurrentPcode()).getBlockIndex(),currProc.getBottom());
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return (getX()==-1?"monitor ":"")+"CALL psize-1 = "+getY();
}
}
