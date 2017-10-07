package baci.gui;

import baci.program.*;
import baci.interpreter.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
/**
 * Manages open windows
 * @author: David Strite
 */
public class WindowManager implements InternalFrameListener {
	private HashMap windows;
	private JDesktopPane desktop;
	private DebuggerFrame debuggerFrame;
	
/**
 * Construct new WindowManager for specified destop
 * @param desktop
 */
public WindowManager(JDesktopPane desktop, DebuggerFrame d) {
	debuggerFrame = d;
	windows = new HashMap();
	this.desktop = desktop;
}

/**
 * Closes all open windows
 */
public void closeAllWindows() {
	Object[] wins = windows.values().toArray();
	for(int i=wins.length-1; i>=0; i--) {
		((BaciWindow)wins[i]).dispose();
	}
	windows.clear();
}
/**
 * Closes all open windows
 */
public void closeProcessWindows(boolean closeMain) {
	Object[] wins = windows.values().toArray();
	ArrayList winsRemove = new ArrayList();
	ProcessWindow pw;
	for(int i=wins.length-1; i>=0; i--) {
		if(wins[i] instanceof ProcessWindow) {
			pw=(ProcessWindow)wins[i];
			if(closeMain || pw.getProcess().getIndex()>0) {
				pw.dispose();
				winsRemove.add(pw);
			}
		}
	}
	for(int i=winsRemove.size()-1; i>=0; i--) {
		pw=(ProcessWindow)winsRemove.get(i);
		windows.remove(pw.getProcess());
	}
}
/**
 * Gets a Linda window if it is open
 * @return LindaWindow
 */
public LindaWindow getLindaWindow() {
	return (LindaWindow)windows.get("Linda");
}
/**
 * Gets a console window if it is open
 * @return ConsoleWindow
 */
public ConsoleWindow getConsoleWindow() {
	return (ConsoleWindow)windows.get("CONSOLE");
}
/**
 * Gets a globals window if it is open
 * @return GlobalsWindow
 */
public GlobalsWindow getGlobalsWindow() {
	return (GlobalsWindow)windows.get("GLOBALS");
}
/**
 * Gets a history window if it is open
 * @return HistoryWindow
 */
public HistoryWindow getHistoryWindow() {
	return (HistoryWindow)windows.get("HISTORY");
}
/**
 * Gets a process window for specified process if it is open
 * @param proc
 * @return ConsoleWindow
 */
public ProcessWindow getProcWindow(BaciProcess proc) {
	return (ProcessWindow)windows.get(proc);
}

public void internalFrameActivated(InternalFrameEvent e){
	BaciWindow window = (BaciWindow)e.getSource();
	if(window instanceof ProcessWindow)
		debuggerFrame.openProcessWindow(((ProcessWindow)window).getProcess().getIndex());
}
/**
 * Called when internal frame is closed
 * @param e event
 */
public void internalFrameClosed(InternalFrameEvent e){
	BaciWindow window = (BaciWindow)e.getSource();
	if(window instanceof ProcessWindow) {
		windows.remove(((ProcessWindow)window).getProcess());
	} else if(window instanceof ConsoleWindow) {
		windows.remove("CONSOLE");
	} else if(window instanceof GlobalsWindow) {
		windows.remove("GLOBALS");
	} else if(window instanceof HistoryWindow) {
		windows.remove("HISTORY");
	} else if(window instanceof LindaWindow) {
		windows.remove("LINDA");
	}
}
public void internalFrameClosing(InternalFrameEvent e){
}
public void internalFrameDeactivated(InternalFrameEvent e){
}
public void internalFrameDeiconified(InternalFrameEvent e){
}
public void internalFrameIconified(InternalFrameEvent e){
}
public void internalFrameOpened(InternalFrameEvent e){
}
/**
 * Opens and activates a Linda window
 * @param debugger
 * @param Linda
 * @return LindaWindow
 */
public LindaWindow showLindaWindow(Debugger debugger, Linda linda) {
	LindaWindow window = (LindaWindow)windows.get("Linda");
	if(window==null) {
		window = new LindaWindow(debugger,linda);
		window.addInternalFrameListener(this);
		desktop.add(window);
		windows.put("Linda",window);
		window.show();
	}
	try {
		window.setSelected(true);
	} catch (java.beans.PropertyVetoException pve) {
	}
	return window;
}
/**
 * Opens and activates a console window
 * @param debugger
 * @param console
 * @return ConsoleWindow
 */
public ConsoleWindow showConsoleWindow(Debugger debugger, Console console) {
	ConsoleWindow window = (ConsoleWindow)windows.get("CONSOLE");
	if(window==null) {
		window = new ConsoleWindow(debugger,console);
		window.addInternalFrameListener(this);
		desktop.add(window);
		windows.put("CONSOLE",window);
		window.show();
	}
	try {
		window.setSelected(true);
	} catch (java.beans.PropertyVetoException pve) {
	}
	return window;
}
/**
 * Opens and activates a globals window
 * @param debugger
 * @param interpreter
 * @return GlobalsWindow
 */
public GlobalsWindow showGlobalsWindow(Debugger debugger, Interpreter interpreter) {
	GlobalsWindow window = (GlobalsWindow)windows.get("GLOBALS");
	if(window==null) {
		window = new GlobalsWindow(debugger,interpreter);
		window.addInternalFrameListener(this);
		desktop.add(window);
		windows.put("GLOBALS",window);
		window.show();
	}
	try {
		window.setSelected(true);
	} catch (java.beans.PropertyVetoException pve) {
	}
	return window;
}
/**
 * Opens and activates a history window
 * @param debugger
 * @param interpreter
 * @return HistoryWindow
 */
public HistoryWindow showHistoryWindow(Debugger debugger, Interpreter interpreter) {
	HistoryWindow window = (HistoryWindow)windows.get("HISTORY");
	if(window==null) {
		window = new HistoryWindow(debugger,interpreter);
		window.addInternalFrameListener(this);
		desktop.add(window);
		windows.put("HISTORY",window);
		window.show();
	}
	try {
		window.setSelected(true);
	} catch (java.beans.PropertyVetoException pve) {
	}
	return window;
}
/**
 * Opens and activates a process window for specified process
 * @param proc
 * @param debugger
 * @return ConsoleWindow
 */
public ProcessWindow showProcWindow(BaciProcess proc, Debugger debugger) {
	ProcessWindow window = (ProcessWindow)windows.get(proc);
	if(window==null) {
		window = new ProcessWindow(proc,debugger);
		window.addInternalFrameListener(this);
		desktop.add(window);
		windows.put(proc,window);
		window.show();
	}
	try {
		window.setSelected(true);
	} catch (java.beans.PropertyVetoException pve) {
		pve.printStackTrace();
	}
	return window;
}
}
