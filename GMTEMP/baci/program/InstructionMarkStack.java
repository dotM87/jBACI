package baci.program;

import baci.interpreter.*;
/**
 * InstructionMarkStack
 * @author: David Strite
 */
public class InstructionMarkStack extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionMarkStack(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	Program program = getProgram();
	//If in cobegin block, start a new process
	if(interpreter.isInCobeginBlock()) {
		ProcessList procList = interpreter.getProcesses();
		int newProcIndex = procList.getSize();
		Identifier identifier = program.getIdentifier(getY());
		BaciProcess newProcess = new BaciProcess(identifier.getName(),newProcIndex,program,interpreter);
		int bottom = 0;
		newProcess.setBottom(bottom);
		newProcess.setTop(bottom-1);
		Stack display = newProcess.getDisplay();
		Stack mainStack = procList.get(0).getStack();
		display.set(0,new Address(procList.get(0),0));
		display.set(1,new Address(procList.get(0),program.getBlock(0).getVsize()));
		procList.add(newProcess);
	}
	
	BaciProcess currProc = interpreter.getCurrentProcess();
	int top = currProc.getTop()+5;
	currProc.setTop(top);
	int size = program.getBlock(program.getIdentifier(getY()).getRef()).getVsize();
	Stack stack = currProc.getStack();
	stack.set(top-1,size-1);
	stack.set(top,getY());
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "MARKSTACK "+getProgram().getIdentifier(getY()).getName();
}
}
