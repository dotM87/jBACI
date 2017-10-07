package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionEnterMonitor
 * @author: David Strite
 */
public class InstructionEnterMonitor extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionEnterMonitor(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if a cross monitor call is encountered or if the monitor has not been initialized
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	Program program = getProgram();
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int bottom = currProc.getBottom();
	int procIdentIndex = stack.getInt(bottom+4);
	Address temp = stack.getAddress(bottom+3);
	int callerIdentIndex = temp.getProcess().getStack().getInt(temp.getIndex()+4);
	
	if (program.getIdentifier(callerIdentIndex).getMon() != 0) {
		/* Problem with calling a monitor procedure from the init code */
		if (program.getIdentifier(callerIdentIndex).getObj() == Identifier.OBJ_TYPE_MONITOR) {
			if (program.getIdentifier(procIdentIndex).getMon() != callerIdentIndex) {
				throw new BaciException("Cannot call a monitor from another monitor");
			}
		} else {
			if (program.getIdentifier(procIdentIndex).getMon() != program.getIdentifier(callerIdentIndex).getMon()) {
				throw new BaciException("Cannot call a monitor from another monitor");
			}
		}
/*
		if (program.getIdentifier(procIdentIndex).getMon() != program.getIdentifier(callerIdentIndex).getMon()) {
			throw new BaciException("Cannot call a monitor from another monitor");
		}
*/
	} else {
		Address address = new Address(interpreter.getProcess(0),program.getIdentifier(getY()).getMon());
		currProc.setMonitor(address, program.getIdentifier(getY()).getName());
		Object value = address.getProcess().getStack().get(address.getIndex());
		if (value == null) {
			throw new BaciException("Monitor has not been initialized");
		} else if (((Integer)value).intValue() == 0) {
			interpreter.setNumInstBeforeSwap(0);
			currProc.setActive(false);
			currProc.setSuspend(address);
		} else {
			int newValue = ((Integer)value).intValue()-1;
			address.getProcess().getStack().set(address.getIndex(),newValue);
		}
	}
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "ENTERMON "+getProgram().getIdentifier(getY()).getName();
}
}
