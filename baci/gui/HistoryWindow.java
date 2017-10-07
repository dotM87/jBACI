package baci.gui;

import baci.interpreter.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
/**
 * Subwindow for history data
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class HistoryWindow extends BaciWindow {
    private javax.swing.JPanel ivjBaciWindowContentPane = null;
    private javax.swing.JScrollPane ivjscpHistory = null;
    private HistoryTable ivjtblHistory = null;  // MB
//  private javax.swing.JTable ivjtblHistory = null;  
    private baci.interpreter.Interpreter interpreter;
    //Maximum number of history records to keep
    public static final int MAX_HISTORY_SIZE = 150;
/**
 * Constructor
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
public HistoryWindow() {
    super();
    initialize();
}
/**
 * Constructs new ConsoleWindow with specified Debugger and Interpreter
 * @param d
 * @param i
 */
public HistoryWindow(Debugger d, Interpreter i) {
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
            ivjBaciWindowContentPane.setBorder(new TitledBorder(Config.MRI_TITLE));
            ivjBaciWindowContentPane.setLayout(new java.awt.BorderLayout());
            getBaciWindowContentPane().add(getscpHistory(), "Center");
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
 * Gets the Interpreter associated with this HistoryWindow
 * @return Interpreter
 */
public baci.interpreter.Interpreter getInterpreter() {
    return interpreter;
}
/**
 * Return the scpHistory property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpHistory() {
    if (ivjscpHistory == null) {
        try {
            ivjscpHistory = new javax.swing.JScrollPane();
            ivjscpHistory.setName("scpHistory");
            ivjscpHistory.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
            ivjscpHistory.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            getscpHistory().setViewportView(gettblHistory());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpHistory;
}
/**
 * Return the tblHistory property value.
 * @return javax.swing.JTable
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTable gettblHistory() {
    if (ivjtblHistory == null) {
        try {
            ivjtblHistory = new HistoryTable(); 
            ivjtblHistory.setName("tblHistory");
            ivjtblHistory.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            getscpHistory().setColumnHeaderView(ivjtblHistory.getTableHeader());
            getscpHistory().getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
            ivjtblHistory.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_OFF);
            ivjtblHistory.setBounds(0, 0, 300, 200);
            ivjtblHistory.setAutoCreateColumnsFromModel(true);
            // user code begin {1}
            if(interpreter!=null) {
                ivjtblHistory.setModel(interpreter.getHistoryModel());

                TableColumn column;
                column=ivjtblHistory.getColumnModel().getColumn(0);
                column.setPreferredWidth(30);
                column=ivjtblHistory.getColumnModel().getColumn(1);
                column.setPreferredWidth(40);
                column=ivjtblHistory.getColumnModel().getColumn(2);
                column.setPreferredWidth(140);
                column=ivjtblHistory.getColumnModel().getColumn(3);
                column.setPreferredWidth(interpreter.getDebugger().getDebuggerFrame().isHistorySource() ? 0 : 140);
                column=ivjtblHistory.getColumnModel().getColumn(4);
                column.setPreferredWidth(70);

                ivjtblHistory.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtblHistory;
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
        setTitle(Config.HISTORY_TITLE);
        setContentPane(getBaciWindowContentPane());
//        setLocation(getDebugger().getDebuggerFrame().newOffsetX(), getDebugger().getDebuggerFrame().newOffsetY());
	setLocation(Config.getIntegerProperty("HISTORY_X"), Config.getIntegerProperty("HISTORY_Y"));    
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
