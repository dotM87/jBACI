package baci.gui;

import baci.interpreter.*;
import baci.program.*;
import javax.swing.border.*;
/**
 * Sub window containing Linda
 * @author: Modified by Moti Ben-Ari
 * See copyright.txt.
 */
public class LindaWindow extends BaciWindow {
    private javax.swing.JPanel ivjBaciWindowContentPane = null;
    private javax.swing.JTextArea ivjtxaLinda = null;
    private Linda linda;
    private javax.swing.JScrollPane ivjscpLinda = null;
/**
 * Constructor
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
public LindaWindow() {
    super();
    initialize();
}
/**
 * Construct new Linda window with specified Debugger and Linda
 * @param d
 * @param c
 */
public LindaWindow(Debugger d, Linda c) {
    super(d);
    linda = c;
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
            getBaciWindowContentPane().add(getscpLinda(), "Center");
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
 * Return the scpLinda property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpLinda() {
    if (ivjscpLinda == null) {
        try {
            ivjscpLinda = new javax.swing.JScrollPane();
            ivjscpLinda.setName("scpLinda");
//            ivjscpLinda.setBorder(new TitledBorder("Output"));
            getscpLinda().setViewportView(gettxaLinda());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpLinda;
}
/**
 * Return the txaLinda property value.
 * @return javax.swing.JTextArea
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextArea gettxaLinda() {
    if (ivjtxaLinda == null) {
        try {
            ivjtxaLinda = new javax.swing.JTextArea();
            ivjtxaLinda.setName("txaLinda");
            ivjtxaLinda.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxaLinda.setBounds(0, 0, 314, 202);
            ivjtxaLinda.setEditable(false);
            // user code begin {1}
            ivjtxaLinda.setDocument(linda);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxaLinda;
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
        setName("LindaWindow");
	setSize(Config.getIntegerProperty("SUBWINDOW_X"), Config.getIntegerProperty("SUBWINDOW_Y"));
//        setSize(Config.SUB_X, Config.SUB_Y);
        setTitle(Config.LINDA_TITLE);
        setContentPane(getBaciWindowContentPane());
	setLocation(Config.getIntegerProperty("LINDA_X"), Config.getIntegerProperty("LINDA_Y"));
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
        LindaWindow aLindaWindow;
        aLindaWindow = new LindaWindow(new baci.gui.Debugger(), new Linda());
        frame.setContentPane(aLindaWindow);
        frame.setSize(aLindaWindow.getSize());
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
