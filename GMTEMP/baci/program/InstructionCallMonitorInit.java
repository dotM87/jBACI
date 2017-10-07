package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionCallMonitorInit
 * @author: David Strite
 */
public class InstructionCallMonitorInit extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionCallMonitorInit(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if called from process other than main process
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	if(currProc.getIndex()!=0) {
		throw new BaciException(getDescription()+" can only be called by main process");
	}
	int index = currProc.getTop()-4;
	int identIndex = stack.getInt(index+4);
	int level = getProgram().getIdentifier(identIndex).getLevel();
	int size = stack.getInt(index+3)+index;
	stack.set(index,1);
	stack.set(index+1,currProc.getCurrentPcode());
	stack.set(index+2,currProc.getDisplay().getAddress(1));
	stack.set(index+3,currProc.getDisplay().getAddress(1));
	currProc.getDisplay().set(level+1,new Address(currProc,index));
	for(int i=currProc.getTop()+1; i<=size; i++) {
		stack.set(i,0);
	}
	getProgram().getIdentifier(identIndex).setMon(index);
	currProc.setTop(size);
	currProc.setBottom(index);
	//Jump to pcode address
	int pcodeAdress = getProgram().getIdentifier(identIndex).getAdr();
	getProgram().validatePcodeAddress(pcodeAdress);
	currProc.setCurrentPcode(pcodeAdress);
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected String getDescription() {
	return "CALL_MONINIT";
}
}
