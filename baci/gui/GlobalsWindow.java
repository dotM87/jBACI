package baci.gui;

import baci.interpreter.*;
import baci.gui.treetable.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.tree.*;
/**
 * Subwindow for globals data
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class GlobalsWindow extends BaciWindow {
	private javax.swing.JPanel ivjBaciWindowContentPane = null;
	private baci.interpreter.Interpreter interpreter;
	private javax.swing.JScrollPane ivjscpGlobals = null;
/**
 * Constructor
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
public GlobalsWindow() {
	super();
	initialize();
}
/**
 * Construct new ConsoleWindow with specified Debugger and Interpreter
 * @param d
 * @param i
 */
public GlobalsWindow(Debugger d, Interpreter i) {
	super(d);
	interpreter = i;
	initialize();
}
/**
 * Return the BaciWindowContentPane property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getBaciWindowContentPane() {
	if (ivjBaciWindowContentPane == null) {
		try {
			ivjBaciWindowContentPane = new javax.swing.JPanel();
			ivjBaciWindowContentPane.setName("BaciWindowContentPane");
			ivjBaciWindowContentPane.setLayout(new java.awt.BorderLayout());
			getBaciWindowContentPane().add(getscpGlobals(), "Center");
			// user code begin {1}
			// user code end
		} catch (java.lang.Throwable ivjExc) {
			// user code begin {2}
			// user code end
			handleException(ivjExc);
		}
	}
	return ivjBaciWindowContentPane;
}
/**
 * Insert the method's description here.
 * Creation date: (9/8/2001 1:14:13 PM)
 * @return baci.interpreter.Interpreter
 */
public baci.interpreter.Interpreter getInterpreter() {
	return interpreter;
}
/**
 * Return the scpGlobals property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpGlobals() {
	if (ivjscpGlobals == null) {
		try {
			ivjscpGlobals = new javax.swing.JScrollPane();
			ivjscpGlobals.setName("scpGlobals");
			ivjscpGlobals.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
			ivjscpGlobals.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
			// user code begin {1}
			if(interpreter!=null) {
				JTreeTable treeTable = new JTreeTable(interpreter.getGlobalVariableModel());
				treeTable.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
					Config.getIntegerProperty("TABLE_FONT_SIZE")));
				ivjscpGlobals.setViewportView(treeTable);
				JTree tree = treeTable.getTree();
				tree.setShowsRootHandles(true);
				DefaultTreeCellRenderer renderer = (DefaultTreeCellRenderer)tree.getCellRenderer();
				renderer.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
					Config.getIntegerProperty("TABLE_FONT_SIZE")));
				renderer.setClosedIcon(null);
				renderer.setDisabledIcon(null);
				renderer.setLeafIcon(null);
				renderer.setOpenIcon(null);
				treeTable.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
			}
			// user code end
		} catch (java.lang.Throwable ivjExc) {
			// user code begin {2}
			// user code end
			handleException(ivjExc);
		}
	}
	return ivjscpGlobals;
}
/**
 * Called whenever the part throws an exception.
 * @param exception java.lang.Throwable
 */
private void handleException(java.lang.Throwable exception) {

	/* Uncomment the following lines to print uncaught exceptions to stdout */
	// System.out.println("--------- UNCAUGHT EXCEPTION ---------");
	// exception.printStackTrace(System.out);

	getDebugger().getDebuggerFrame().showError(exception);
}
/**
 * Initialize the class.
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void initialize() {
	try {
		// user code begin {1}
		// user code end
		setName("ConsoleWindow");
		setSize(Config.getIntegerProperty("SUBWINDOW_X"), Config.getIntegerProperty("SUBWINDOW_Y"));
//		setSize(Config.SUB_X, Config.SUB_Y);
		setTitle(Config.GLOBALS_TITLE);
		setContentPane(getBaciWindowContentPane());
//		setLocation(getDebugger().getDebuggerFrame().newOffsetX(), getDebugger().getDebuggerFrame().newOffsetY());
		setLocation(Config.getIntegerProperty("GLOBALS_X"), Config.getIntegerProperty("GLOBALS_Y"));
	} catch (java.lang.Throwable ivjExc) {
		handleException(ivjExc);
	}
	// user code begin {2}
	// user code end
}
/**
 * main entrypoint - starts the part when it is run as an application
 * @param args java.lang.String[]
 */
public static void main(java.lang.String[] args) {
	try {
		javax.swing.JFrame frame = new javax.swing.JFrame();
		ConsoleWindow aConsoleWindow;
		aConsoleWindow = new ConsoleWindow(new baci.gui.Debugger(), new baci.interpreter.Console());
		frame.setContentPane(aConsoleWindow);
		frame.setSize(aConsoleWindow.getSize());
		frame.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				System.exit(0);
			};
		});
		frame.show();
		java.awt.Insets insets = frame.getInsets();
		frame.setSize(frame.getWidth() + insets.left + insets.right, frame.getHeight() + insets.top + insets.bottom);
		frame.setVisible(true);
	} catch (Throwable exception) {
		System.err.println("Exception occurred in main() of baci.gui.BaciWindow");
		exception.printStackTrace(System.out);
	}
}
}
