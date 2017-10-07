package baci.gui;

import baci.interpreter.*;
import javax.swing.border.*;
/**
 * Sub window containing console
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class ConsoleWindow extends BaciWindow {
    private javax.swing.JPanel ivjBaciWindowContentPane = null;
    private javax.swing.JTextArea ivjtxaConsole = null;
    private baci.interpreter.Console console;
    private javax.swing.JScrollPane ivjscpConsole = null;
/**
 * Constructor
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
public ConsoleWindow() {
    super();
    initialize();
}
/**
 * Construct new Console window with specified Debugger and Console
 * @param d
 * @param c
 */
public ConsoleWindow(Debugger d, Console c) {
    super(d);
    console = c;
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
            getBaciWindowContentPane().add(getscpConsole(), "Center");
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
 * Return the scpConsole property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpConsole() {
    if (ivjscpConsole == null) {
        try {
            ivjscpConsole = new javax.swing.JScrollPane();
            ivjscpConsole.setName("scpConsole");
//            ivjscpConsole.setBorder(new TitledBorder("Output"));
            getscpConsole().setViewportView(gettxaConsole());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpConsole;
}
/**
 * Return the txaConsole property value.
 * @return javax.swing.JTextArea
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextArea gettxaConsole() {
    if (ivjtxaConsole == null) {
        try {
            ivjtxaConsole = new javax.swing.JTextArea();
            ivjtxaConsole.setName("txaConsole");
            ivjtxaConsole.setFont(new java.awt.Font(Config.SOURCE_FONT_FAMILY, Config.SOURCE_FONT_STYLE, 
	    		   Config.getIntegerProperty("SOURCE_FONT_SIZE")));
            ivjtxaConsole.setBounds(0, 0, 314, 202);
            ivjtxaConsole.setEditable(false);
            // user code begin {1}
            ivjtxaConsole.setDocument(console);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxaConsole;
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
//        setSize(Config.SUB_X, Config.SUB_Y);
        setTitle(Config.CONSOLE_TITLE);
        setContentPane(getBaciWindowContentPane());
	setLocation(Config.getIntegerProperty("CONSOLE_X"), Config.getIntegerProperty("CONSOLE_Y"));
//        setLocation(getDebugger().getDebuggerFrame().newOffsetX(), getDebugger().getDebuggerFrame().newOffsetY());
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
