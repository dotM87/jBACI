package baci.program;

import baci.exception.*;
import baci.interpreter.*;
/**
 * InstructionRemove
 * @author: Moti Ben-Ari
 */
public class InstructionRemove extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionRemove(Program program, int x, int y) {
    super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if the value of binary semaphore is invalid
 */
public void doInstruction(Interpreter interpreter) throws BaciException{
    BaciProcess currProc = interpreter.getCurrentProcess();
    Stack stack = currProc.getStack();
    Linda linda = interpreter.getLinda();

    int top = currProc.getTop();
    int v1 = stack.getInt(top-2);
    int v2 = Linda.formal, v3 = Linda.formal;
    // Additional parameters may be Linda.formal instead of an address
    boolean oneParm = !(stack.get(top) instanceof Address);
    boolean noParm  = !(stack.get(top-1) instanceof Address);
    Address address2 = null, address3 = null;
    if (!noParm) {
	    address2 = stack.getAddress(top-1);
	    v2 = address2.getProcess().getStack().getInt(address2.getIndex());
    }
    if (!noParm && !oneParm) {
	    address3 = stack.getAddress(top);
	    v3 = address3.getProcess().getStack().getInt(address3.getIndex());
    }
    // Remove if x = 0, Read if x = 1
    // y indicates value or var parameter: 0=(value,value), 1=(var,value), 2=(value,var), 3=(var,var)
    // Replace actual value of v2/v3 by Linda.formal so that it will match anything for value parameter
    if ((getY() % 2) == 0) { v2 = Linda.formal; }
    if (getY() < 2) { v3 = Linda.formal; }
    Note note = linda.readremove(currProc, v1, v2, v3, (getX() == 0));
    // If note returned is null, suspend this process, otherwise, return values from note
    if (note == null) {
		currProc.setSuspend(null);
		currProc.setActive(false);
		interpreter.setNumInstBeforeSwap(0);
    }
    else {
	    if (!noParm) 
		    address2.getProcess().getStack().set(address2.getIndex(), note.note[1]);
	    if (!noParm && !oneParm) 
		    address3.getProcess().getStack().set(address3.getIndex(), note.note[2]);
	    currProc.setTop(top-3);
    }
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
    return "REMOVE, pop(3)";
}
}
