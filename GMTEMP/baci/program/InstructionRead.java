package baci.program;

import baci.exception.*;
import javax.swing.*;
import baci.gui.*;
import baci.interpreter.*;
/**
 * InstructionRead
 * @author: David Strite
 * @author: Moti Ben-Ari, 2003.
 * Modified for non-blocking read.
 */
public class InstructionRead extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionRead(Program program, int x, int y) {
	super(program, x, y);
}

/* For non-blocking read, create a thread to wait on the JOptionPane */

InputThread inputThread = null;		// The thread

class InputThread extends Thread { 
	String inp = null;
	public void run() {
	   JOptionPane pane = new JOptionPane("", JOptionPane.PLAIN_MESSAGE, JOptionPane.DEFAULT_OPTION);
	   JDialog dialog = pane.createDialog(null, Config.INPUT_TITLE);
	   pane.setWantsInput(true);
	   dialog.setModal(true);
	   dialog.setBounds(Config.getIntegerProperty("INPUT_X"), Config.getIntegerProperty("INPUT_Y"), 200, 100);
           while(inp == null || (inp.equals("")&&getY()==Identifier.DATA_TYPE_INTS)) {
		   dialog.show();
		   Object selectedValue = pane.getInputValue();
		   if (!selectedValue.equals(javax.swing.JOptionPane.UNINITIALIZED_VALUE.toString())) 
			   inp = (String) selectedValue;
	   }
	}

	String getInput() { return inp;}
}

/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 * @throws BaciException if invalid data type is specified
 */
public void doInstruction(Interpreter interpreter) throws BaciException {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();
	int top = currProc.getTop();
	Address address = stack.getAddress(top);
	String input = null;
	final int iflag = -32767; 
	final int cflag = (int) '\r';
	int flag = getY() == Identifier.DATA_TYPE_INTS ? iflag : cflag;
//	if (currProc.getIndex() == 0) {  // For main program, use blocking read
	if (getX() == 0) {  // Blocking read
		while(true) {
			input = (String) JOptionPane.showInputDialog(null);
			if ((input == null) || (input.equals(""))) continue;
			switch (getY()) {
			case Identifier.DATA_TYPE_INTS:
				int n = flag;
				try { n = Integer.parseInt(input.trim()); }
				catch (NumberFormatException e) { continue; };			
				address.getProcess().getStack().set(address.getIndex(), n);
				break;
			case Identifier.DATA_TYPE_CHARS:
				int nn = input.charAt(0);
				interpreter.setReadNewLine(nn == '\n');
				address.getProcess().getStack().set(address.getIndex(),nn);
				break;
			default:
				break;
			}
			break;
		}
	}
	else if (inputThread == null) {			// First time, no thread
		inputThread = new InputThread();
		inputThread.start();
		address.getProcess().getStack().set(address.getIndex(),flag);
	} 
	else if (inputThread.getInput() == null) { // Next times, check for input
		address.getProcess().getStack().set(address.getIndex(),flag);
	}
	else {
		input = inputThread.getInput();
		inputThread = null;
		switch (getY()) {
			case Identifier.DATA_TYPE_INTS:
				int n = flag;
				try { n = Integer.parseInt(input); }
				catch (NumberFormatException e) {};			
				address.getProcess().getStack().set(address.getIndex(), n);
				break;
			case Identifier.DATA_TYPE_CHARS:
				int nn = input.equals("") ? flag : input.charAt(0);
				interpreter.setReadNewLine(nn == '\n');
				address.getProcess().getStack().set(address.getIndex(),nn);
				break;
			default:
				throw new BaciException("Invalid data for READ");
		}
	}
	currProc.setTop(top-1);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return (getX()==0?"READ ":"NB READ")+(getY()==Identifier.DATA_TYPE_INTS?"int":"char")+" to &s[t], pop(1)";
}

}
