package baci.gui;

import baci.exception.*;
import baci.interpreter.*;
import baci.program.*;
import java.io.*;

/**
 * Class that is run from the command line to start the debugger. This then starts the other components.
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class Debugger {
    private DebuggerFrame debuggerFrame;
    private Interpreter interpreter;
/**
 * Construct a new Debugger
 */
public Debugger() {
    interpreter = new Interpreter(this);
    debuggerFrame = new DebuggerFrame(this);
    debuggerFrame.show();
}
/**
 * Gets the DebuggerFrame
 * @return DebuggerFrame
 */
public DebuggerFrame getDebuggerFrame() {
    return debuggerFrame;
}
/**
 * Gets the Interpreter
 * @return Interpreter
 */
public Interpreter getInterpreter() {
    return interpreter;
}
/**
 * Starts the application.
 * @param args an array of command-line arguments
 */

public static void main(java.lang.String[] args) {
    final String a = (args.length>0) ? args[0] : "";
    javax.swing.SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		    Config.init();
		    Debugger d = new Debugger();
		    if(!a.equals("")) {
			    d.getDebuggerFrame().getEditor().openFile(new java.io.File(a));
		    }
		    d.getDebuggerFrame().newInitialize(false);
	    }
    });
}
/**
 * Opens a pcode file and starts the interpreter
 * @param File
 */
public void startPcodeFile(File pcodeFile) {
    try {
        Program prog = new Program(pcodeFile);
        interpreter.initialize(prog);
        debuggerFrame.getWindowManager().closeAllWindows();
        interpreter.addCurrentProcessListener(debuggerFrame);
    } catch (Exception e) {
        getDebuggerFrame().showError(e);
    }
}
/**
 * Stops the debugger
 */
public void stop() {
    System.exit(0);
}
}
