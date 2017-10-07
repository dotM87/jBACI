package baci.gui;

import javax.swing.*;
/**
 * Super class of BACI subwindows
 * @author: David Strite
 */
public abstract class BaciWindow extends JInternalFrame {
	private Debugger debugger;
/**
 * Construct a new BaciWindow
 */
public BaciWindow() {
}
/**
 * Construct a new BaciWindow with specified Debugger
 * @param d
 */
public BaciWindow(Debugger d) {
	debugger = d;
	setResizable(true);
	setIconifiable(true);
	setMaximizable(true);
	setClosable(true);
}
/**
 * Get debugger associated with this window
 * @return Debugger
 */
public Debugger getDebugger() {
	return debugger;
}
/**
 * Set the debugger associated with this window
 * @param newDebugger
 */
public void setDebugger(Debugger newDebugger) {
	debugger = newDebugger;
}
}
