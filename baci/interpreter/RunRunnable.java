package baci.interpreter;

import javax.swing.*;
/**
 * Runnable used by run thread
 * @author: David Strite
 */
public class RunRunnable implements Runnable {
	private Interpreter interpreter;
/**
 * Constructs a new RunRunnable with specified Interpreter
 * @param interpreter
 */
public RunRunnable(Interpreter interpreter) {
	this.interpreter = interpreter;
}
/**
 * Gets the interpreter for this RunRunnable
 * @return Interpreter
 */
public Interpreter getInterpreter() {
	return interpreter;
}
/**
 * Called by run tread
 */
public void run() {
	try {
		while (interpreter.getStatus()==Interpreter.STATUS_RUN) {
			//Wait for interpreter to be running 
			interpreter.waitRun();
			try {
				//Have interpreter execute one instruction
				SwingUtilities.invokeAndWait(interpreter);
			} catch(InterruptedException ie) {
			}
		}
	} catch(Throwable t) {
		interpreter.getDebugger().getDebuggerFrame().showError(t);
	}
}
}
