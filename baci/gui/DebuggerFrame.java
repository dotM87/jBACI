package baci.gui;

import baci.event.*;
import baci.gui.*;
import baci.interpreter.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

/**
 * Main Debugger Window
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */

public class DebuggerFrame extends JFrame implements CurrentProcessListener {
    private JDesktopPane ivjdskSubWindowArea = null;
    private Editor editor = null;
    private JScrollPane scpEditor = null;
    private JPanel ivjJFrameContentPane = null;
    private JTable ivjlstProcesses = null;   // Retain name lst even though now table
    private JSplitPane ivjsplMain = null;
    private JMenuBar ivjDebuggerFrameJMenuBar = null;
    private JMenu ivjmnuFile = null;
    private JMenu ivjmnuProgram = null;
    private JToolBar ivjtlbToolBar = null;
    private baci.gui.actionbuttons.GoButton ivjbtnGo = null;
    private baci.gui.actionbuttons.PauseButton ivjbtnPause = null;
    private Debugger debugger;
    IvjEventHandler ivjEventHandler = new IvjEventHandler();
    private baci.gui.actionbuttons.ExitButton ivjbtnExit = null;
    private baci.gui.actionbuttons.OpenButton ivjbtnOpen = null;
    private JMenu ivjmnuHelp = null; 
    private baci.gui.actionbuttons.AboutButton ivjbtnAbout = null;
    private JScrollPane ivjscpProcesses = null;
    private WindowManager windowManager;
    private JMenu ivjmnuWindow = null;
    private baci.gui.actionbuttons.ConsoleButton ivjbtnConsole = null;
    private baci.gui.actionbuttons.LindaButton ivjbtnLinda = null;
    private baci.gui.actionbuttons.PcodeStepButton ivjbtnPcodeStep = null;
    private baci.gui.actionbuttons.SourceStepButton ivjbtnSourceStep = null;
    private baci.gui.actionbuttons.GlobalsButton ivjbtnGlobals = null;
    private JCheckBoxMenuItem ivjmncStopOnSwap = null;
    private JMenu ivjmnuOptions = null;
    private JCheckBoxMenuItem ivjmncShowActive = null;
    private boolean showActiveWindow;
    private JCheckBoxMenuItem ivjmncHistorySource = null;
    private boolean historySource;
    private JCheckBoxMenuItem ivjmncHistoryFile = null;
    private boolean historyFile;
    private boolean saveShowActiveWindow; // So that feature can be turned off during Go
    private JButton ivjbtnOpenProc = null;
    private JButton ivjbtnNextProc = null;
    private JButton ivjbtnPrevProc = null;
    private JPanel ivjpnlProcesses = null;
    private JPanel ivjpnlProcessButton = null;
    private baci.gui.actionbuttons.HistoryButton ivjbtnHistory = null;
    private baci.gui.actionbuttons.RunButton ivjbtnRun = null;
    private baci.gui.actionbuttons.NewButton ivjbtnNew = null;
    private baci.gui.actionbuttons.SaveButton ivjbtnSave = null;
    private baci.gui.actionbuttons.SaveAsButton ivjbtnSaveAs = null;
    private JMenu ivjmnuEditor = null;
    private baci.gui.actionbuttons.CopyButton ivjbtnCopy = null;
    private baci.gui.actionbuttons.CutButton ivjbtnCut = null;
    private baci.gui.actionbuttons.PasteButton ivjbtnPaste = null;
    private baci.gui.actionbuttons.FindButton ivjbtnFind = null;
    private baci.gui.actionbuttons.FindAgainButton ivjbtnFindAgain = null;
    private baci.gui.actionbuttons.EditButton ivjbtnEdit = null;
    private baci.gui.actionbuttons.CompileButton ivjbtnCompile = null;
    private ProcessTableModel processTableModel;
    private int offsetX;  // Offsets for successive subwindows
    private int offsetY;
    private java.awt.DefaultKeyboardFocusManager focusManager = new java.awt.DefaultKeyboardFocusManager();

class IvjEventHandler implements 
  java.awt.event.ActionListener, 
  java.awt.event.MouseListener, 
  java.awt.event.WindowListener {

        public void actionPerformed(java.awt.event.ActionEvent e) {
            if (e.getSource() == DebuggerFrame.this.getmncStopOnSwap()) 
                connEtoC3(e);
            if (e.getSource() == DebuggerFrame.this.getmncShowActive()) 
                connEtoC4(e);
            if (e.getSource() == DebuggerFrame.this.getbtnOpenProc()) 
                connEtoC5(e);
            if (e.getSource() == DebuggerFrame.this.getbtnNextProc()) 
                connEtoC6(e);
            if (e.getSource() == DebuggerFrame.this.getbtnPrevProc()) 
                connEtoC7(e);
            if (e.getSource() == DebuggerFrame.this.getmncHistorySource()) 
                connEtoC8(e);
            if (e.getSource() == DebuggerFrame.this.getmncHistoryFile()) 
                connEtoC9(e);
        };
        public void mouseClicked(java.awt.event.MouseEvent e) {
            if (e.getSource() == DebuggerFrame.this.getlstProcesses()) 
                connEtoC2(e);
        };
        public void mouseEntered(java.awt.event.MouseEvent e) {};
        public void mouseExited(java.awt.event.MouseEvent e) {};
        public void mousePressed(java.awt.event.MouseEvent e) {};
        public void mouseReleased(java.awt.event.MouseEvent e) {};
        public void windowActivated(java.awt.event.WindowEvent e) {};
        public void windowClosed(java.awt.event.WindowEvent e) {
            if (e.getSource() == DebuggerFrame.this) 
                connEtoC1(e);
        };
        public void windowClosing(java.awt.event.WindowEvent e) {};
        public void windowDeactivated(java.awt.event.WindowEvent e) {};
        public void windowDeiconified(java.awt.event.WindowEvent e) {};
        public void windowIconified(java.awt.event.WindowEvent e) {};
        public void windowOpened(java.awt.event.WindowEvent e) {};
    };

/**
 * Constructor
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
public DebuggerFrame() {
    super();
    initialize();
}
/**
 * Constructs a new DebuggerFrame with specified Debugger
 * @param d
 */
public DebuggerFrame(Debugger d) {
    super();
    debugger = d;
    initialize();
    windowManager = new WindowManager(getdskSubWindowArea(), this);
}
/**
 * Stops the debugger
 */
public void close() {
    debugger.stop();
}

/**
 * This method is used to switch between edit and interpret mode
 */
public void newInitialize(boolean interpreting) {
    if (interpreting)
        switchToInterpreter();
    else
        switchToEditor();
}

/**
 * Set interpreter contents in pane and enable/disable relevant buttons
 */
public void switchToInterpreter() {
    getbtnLinda().setEnabled(true);
    getbtnConsole().setEnabled(true);
    getbtnGlobals().setEnabled(true);
    getbtnHistory().setEnabled(true);
//    getbtnOpenProc().setEnabled(true);
    getbtnNextProc().setEnabled(true);
    getbtnPrevProc().setEnabled(true);
    getbtnPause().setEnabled(true);
    getbtnPcodeStep().setEnabled(true);
    getbtnRun().setEnabled(true);
    getbtnSourceStep().setEnabled(true);
    getbtnGo().setEnabled(true);
    getbtnEdit().setEnabled(true);

    getmnuEditor().setEnabled(false);
    getbtnCompile().setEnabled(false);
    getbtnOpen().setEnabled(false);
    getbtnNew().setEnabled(false);
    getbtnSave().setEnabled(false);
    getbtnSaveAs().setEnabled(false);
    getbtnCopy().setEnabled(false);
    getbtnCut().setEnabled(false);
    getbtnPaste().setEnabled(false);
    getbtnFind().setEnabled(false);
    getbtnFindAgain().setEnabled(false);

    debugger.getInterpreter().getGraphicsList().clear();
    debugger.getInterpreter().getLinda().clearBoard();
    setShowActiveWindow(getmncShowActive().isSelected());
    offsetX = -Config.OFF_X;  // Reset offsets
    offsetY = -Config.OFF_Y;

    if (Config.getBooleanProperty("INITIAL_GLOBALS"))
	    windowManager.showGlobalsWindow(debugger, debugger.getInterpreter());
    if (Config.getBooleanProperty("INITIAL_CONSOLE"))
	    windowManager.showConsoleWindow(debugger, debugger.getInterpreter().getConsole());
    if (Config.getBooleanProperty("INITIAL_LINDA"))
	    windowManager.showLindaWindow(debugger, debugger.getInterpreter().getLinda());
    if (Config.getBooleanProperty("INITIAL_HISTORY"))
	    windowManager.showHistoryWindow(debugger, debugger.getInterpreter());
    if (Config.getBooleanProperty("OPEN_MAIN_BY_DEFAULT")) 
	    openProcessWindow(0);

    getJFrameContentPane().removeAll();
    getJFrameContentPane().add(getsplMain(), "Center");
    getJFrameContentPane().add(gettlbToolBar(), "North");
    getJFrameContentPane().validate();
    getJFrameContentPane().repaint();
    focusManager.clearGlobalFocusOwner();  // I have no idea why this works, but it does :-)
}

/**
 * Set editor contents in pane and enable/disable relevant buttons
 */
public void switchToEditor() {
    debugger.getInterpreter().setRunning(false);
    debugger.getInterpreter().getGraphicsList().clear();
	
    getbtnLinda().setEnabled(false);
    getbtnConsole().setEnabled(false);
    getbtnGlobals().setEnabled(false);
    getbtnHistory().setEnabled(false);
//    getbtnOpenProc().setEnabled(false);
    getbtnNextProc().setEnabled(false);
    getbtnPrevProc().setEnabled(false);
    getbtnPause().setEnabled(false);
    getbtnPcodeStep().setEnabled(false);
    getbtnSourceStep().setEnabled(false);
    getbtnGo().setEnabled(false);
    getbtnEdit().setEnabled(false);

    getbtnRun().setEnabled(true);
    getmnuEditor().setEnabled(true);
    getbtnCompile().setEnabled(true);
    getbtnOpen().setEnabled(true);
    getbtnNew().setEnabled(true);
    getbtnSave().setEnabled(true);
    getbtnSaveAs().setEnabled(true);
    getbtnCopy().setEnabled(true);
    getbtnCut().setEnabled(true);
    getbtnPaste().setEnabled(true);
    getbtnFind().setEnabled(true);
    getbtnFindAgain().setEnabled(true);

    getJFrameContentPane().removeAll();
    getJFrameContentPane().add(getscpEditor(), "Center");
    getJFrameContentPane().add(gettlbToolBar(), "North");
    getJFrameContentPane().validate();
    getJFrameContentPane().repaint();
    getEditor().focus(0);
}

/**
 * connEtoC1:  (DebuggerFrame.window.windowClosed(java.awt.event.WindowEvent) --> DebuggerFrame.close(Ljava.awt.event.WindowEvent;)V)
 * @param arg1 java.awt.event.WindowEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC1(java.awt.event.WindowEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.close();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC2:  (lstProcesses.mouse.mouseClicked(java.awt.event.MouseEvent) --> DebuggerFrame.processSelected(Ljava.awt.event.MouseEvent;)V)
 * @param arg1 java.awt.event.MouseEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC2(java.awt.event.MouseEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.processSelected(arg1);
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC3:  (mncStopOnSwap.action.actionPerformed(java.awt.event.ActionEvent) --> DebuggerFrame.setStopOnSwap(Ljava.awt.event.ActionEvent;)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC3(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.setStopOnSwap();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC4:  (mncShowActive.action.actionPerformed(java.awt.event.ActionEvent) --> DebuggerFrame.setShowActiveWindow(Z)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC4(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.setShowActiveWindow(getmncShowActive().isSelected());
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}

/**
 * connEtoC5:  (btnOpenProc.action.actionPerformed(java.awt.event.ActionEvent) --> DebuggerFrame.openProcessWindow(I)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC5(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.openProcessWindow(getlstProcesses().getSelectedRow());
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}

/**
 * connEtoC6:  (btnNextProc.action.actionPerformed(java.awt.event.ActionEvent) -->  DebuggerFrame.changeProcessWindow(I)V))
 * @param arg1 java.awt.event.ActionEvent
 */
private void connEtoC6(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.changeProcessWindow(1);
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}

/**
 * connEtoC7:  (btnPrevProc.action.actionPerformed(java.awt.event.ActionEvent) -->  DebuggerFrame.changeProcessWindow(I)V))
 * @param arg1 java.awt.event.ActionEvent
 */
private void connEtoC7(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.changeProcessWindow(-1);
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}

private void connEtoC8(java.awt.event.ActionEvent arg1) {
    try {
        historySource = getmncHistorySource().isSelected();
    } catch (java.lang.Throwable ivjExc) {
        handleException(ivjExc);
    }
}

private void connEtoC9(java.awt.event.ActionEvent arg1) {
    try {
        historyFile = getmncHistoryFile().isSelected();
    } catch (java.lang.Throwable ivjExc) {
        handleException(ivjExc);
    }
}

/**
 *  Show process window i
 */
public void showActive(int i) {
    if(showActiveWindow) {
        windowManager.showProcWindow(debugger.getInterpreter().getProcess(i),debugger);
    }
}

/**
 * Called when the current process changes
 * @param e event
 */
public void currentProcessChanged(CurrentProcessChangeEvent e) {
    getlstProcesses().clearSelection();
    getlstProcesses().setRowSelectionInterval(e.getNewCurrent(),e.getNewCurrent());
    showActive(e.getNewCurrent());
}

/**
 * Increment and return subwindow X offset
 */
/*
public int newOffsetX() {
    offsetX = (offsetX + Config.OFF_X) % Config.SUB_X;
    return offsetX;
}
*/

/**
 * Increment and return subwindow Y offset
 */
/*
public int newOffsetY() {
    offsetY = (offsetY + Config.OFF_Y) % Config.SUB_Y;
    return offsetY;
}
*/

/**
 * Create/return editor object
 */
public Editor getEditor() {
    if (editor == null) editor = new Editor(getscpEditor());
    return editor;
}

/**
 *  Create/return editor scroll pane
 */
private JScrollPane getscpEditor() {
    if (scpEditor == null) {
       scpEditor = new JScrollPane();
       scpEditor.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
       scpEditor.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
//       scpEditor.setViewportView(getEditor());
    }
    return scpEditor;
}

/**
 * Return the btnConsole property value.
 * @return baci.gui.actionbuttons.ConsoleButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.ConsoleButton getbtnConsole() {
    if (ivjbtnConsole == null) {
        try {
            ivjbtnConsole = new baci.gui.actionbuttons.ConsoleButton();
           // user code begin {1}
            ivjbtnConsole.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnConsole;
}
/**
 * Return the btnLinda property value.
 * @return baci.gui.actionbuttons.LindaButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.LindaButton getbtnLinda() {
    if (ivjbtnLinda == null) {
        try {
            ivjbtnLinda = new baci.gui.actionbuttons.LindaButton();
           // user code begin {1}
            ivjbtnLinda.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnLinda;
}
/**
 * Return the btnExit property value.
 * @return baci.gui.actionbuttons.ExitButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.ExitButton getbtnExit() {
    if (ivjbtnExit == null) {
        try {
            ivjbtnExit = new baci.gui.actionbuttons.ExitButton();
            // user code begin {1}
            ivjbtnExit.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnExit;
}
/**
 * Return the btnGlobals property value.
 * @return baci.gui.actionbuttons.GlobalsButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.GlobalsButton getbtnGlobals() {
    if (ivjbtnGlobals == null) {
        try {
            ivjbtnGlobals = new baci.gui.actionbuttons.GlobalsButton();
            // user code begin {1}
            ivjbtnGlobals.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnGlobals;
}
/**
 * Return the btnHistory property value.
 * @return baci.gui.actionbuttons.HistoryButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.HistoryButton getbtnHistory() {
    if (ivjbtnHistory == null) {
        try {
            ivjbtnHistory = new baci.gui.actionbuttons.HistoryButton();
            // user code begin {1}
            ivjbtnHistory.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnHistory;
}

/**
 * Return the btnAbout property value.
 * @return baci.gui.actionbuttons.AboutButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.AboutButton getbtnAbout() {
    if (ivjbtnAbout == null) {
        try {
            ivjbtnAbout = new baci.gui.actionbuttons.AboutButton();
            // user code begin {1}
            ivjbtnAbout.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnAbout;
}

/**
 * Return the btnOpen property value.
 * @return baci.gui.actionbuttons.OpenButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.OpenButton getbtnOpen() {
    if (ivjbtnOpen == null) {
        try {
            ivjbtnOpen = new baci.gui.actionbuttons.OpenButton();
            // user code begin {1}
            ivjbtnOpen.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnOpen;
}
/**
 * Return the btnOpenProc property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnOpenProc() {
    if (ivjbtnOpenProc == null) {
        try {
            ivjbtnOpenProc = new javax.swing.JButton();
            ivjbtnOpenProc.setName("btnOpenProc");
            ivjbtnOpenProc.setText(Config.OpenProc);
            ivjbtnOpenProc.setEnabled(false);
            ivjbtnOpenProc.setMnemonic(Config.OpenProcMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnOpenProc;
}

/**
 * Return the btnNextProc property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnNextProc() {
    if (ivjbtnNextProc == null) {
        try {
            ivjbtnNextProc = new javax.swing.JButton();
            ivjbtnNextProc.setName("btnNextProc");
            ivjbtnNextProc.setText(Config.NextProc);
            ivjbtnNextProc.setEnabled(false);
            ivjbtnNextProc.setMnemonic(Config.NextProcMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnNextProc;
}
/**
 * Return the btnPrevProc property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnPrevProc() {
    if (ivjbtnPrevProc == null) {
        try {
            ivjbtnPrevProc = new javax.swing.JButton();
            ivjbtnPrevProc.setName("btnPrevProc");
            ivjbtnPrevProc.setText(Config.PrevProc);
            ivjbtnPrevProc.setEnabled(false);
            ivjbtnPrevProc.setMnemonic(Config.PrevProcMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnPrevProc;
}

/**
 * Return the btnPause property value.
 * @return baci.gui.actionbuttons.PauseButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.PauseButton getbtnPause() {
    if (ivjbtnPause == null) {
        try {
            ivjbtnPause = new baci.gui.actionbuttons.PauseButton();
            // user code begin {1}
            ivjbtnPause.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnPause;
}
/**
 * Return the btnPcodeStep property value.
 * @return baci.gui.actionbuttons.PcodeStepButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.PcodeStepButton getbtnPcodeStep() {
    if (ivjbtnPcodeStep == null) {
        try {
            ivjbtnPcodeStep = new baci.gui.actionbuttons.PcodeStepButton();
            // user code begin {1}
            ivjbtnPcodeStep.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnPcodeStep;
}
/**
 * Return the btnGo property value.
 * @return baci.gui.actionbuttons.GoButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.GoButton getbtnGo() {
    if (ivjbtnGo == null) {
        try {
            ivjbtnGo = new baci.gui.actionbuttons.GoButton();
            // user code begin {1}
            ivjbtnGo.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnGo;
}

/**
 * Return the btnRun property value.
 * @return baci.gui.actionbuttons.RunButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.RunButton getbtnRun() {
    if (ivjbtnRun == null) {
        try {
            ivjbtnRun = new baci.gui.actionbuttons.RunButton();
            // user code begin {1}
            ivjbtnRun.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnRun;
}

/**
 * Create/return New button
 */
private baci.gui.actionbuttons.NewButton getbtnNew() {
    if (ivjbtnNew == null) { try {
            ivjbtnNew = new baci.gui.actionbuttons.NewButton();
            ivjbtnNew.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnNew;
}

/**
 * Create/return Save button
 */
private baci.gui.actionbuttons.SaveButton getbtnSave() {
    if (ivjbtnSave == null) { try {
            ivjbtnSave = new baci.gui.actionbuttons.SaveButton();
            ivjbtnSave.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnSave;
}

/**
 * Create/return Save as button
 */
private baci.gui.actionbuttons.SaveAsButton getbtnSaveAs() {
    if (ivjbtnSaveAs == null) { try {
            ivjbtnSaveAs = new baci.gui.actionbuttons.SaveAsButton();
            ivjbtnSaveAs.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnSaveAs;
}

/**
 * Create/return Copy button
 */
private baci.gui.actionbuttons.CopyButton getbtnCopy() {
    if (ivjbtnCopy == null) { try {
            ivjbtnCopy = new baci.gui.actionbuttons.CopyButton();
            ivjbtnCopy.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnCopy;
}

/**
 * Create/return Cut button
 */
private baci.gui.actionbuttons.CutButton getbtnCut() {
    if (ivjbtnCut == null) { try {
            ivjbtnCut = new baci.gui.actionbuttons.CutButton();
            ivjbtnCut.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnCut;
}

/**
 * Create/return Paste button
 */
private baci.gui.actionbuttons.PasteButton getbtnPaste() {
    if (ivjbtnPaste == null) { try {
            ivjbtnPaste = new baci.gui.actionbuttons.PasteButton();
            ivjbtnPaste.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnPaste;
}

/**
 * Create/return Find button
 */
private baci.gui.actionbuttons.FindButton getbtnFind() {
    if (ivjbtnFind == null) { try {
            ivjbtnFind = new baci.gui.actionbuttons.FindButton();
            ivjbtnFind.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnFind;
}

/**
 * Create/return Find again button
 */
private baci.gui.actionbuttons.FindAgainButton getbtnFindAgain() {
    if (ivjbtnFindAgain == null) { try {
            ivjbtnFindAgain = new baci.gui.actionbuttons.FindAgainButton();
            ivjbtnFindAgain.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnFindAgain;
}

/**
 * Create/return Edit button
 */
private baci.gui.actionbuttons.EditButton getbtnEdit() {
    if (ivjbtnEdit == null) { try {
            ivjbtnEdit = new baci.gui.actionbuttons.EditButton();
            ivjbtnEdit.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnEdit;
}

/**
 * Create/return Compile button
 */
private baci.gui.actionbuttons.CompileButton getbtnCompile() {
    if (ivjbtnCompile == null) { try {
            ivjbtnCompile = new baci.gui.actionbuttons.CompileButton();
            ivjbtnCompile.setDebugger(debugger);
        } catch (java.lang.Throwable ivjExc) {handleException(ivjExc);}
    } return ivjbtnCompile;
}

/**
 * Return the btnSourceStep property value.
 * @return baci.gui.actionbuttons.SourceStepButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private baci.gui.actionbuttons.SourceStepButton getbtnSourceStep() {
    if (ivjbtnSourceStep == null) {
        try {
            ivjbtnSourceStep = new baci.gui.actionbuttons.SourceStepButton();
            // user code begin {1}
            ivjbtnSourceStep.setDebugger(debugger);
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnSourceStep;
}
/**
 * Gets Debugger associated with this DebuggerFrame
 * @return Debugger
 */
public Debugger getDebugger() {
    return debugger;
}
/**
 * Return the DebuggerFrameJMenuBar property value.
 * @return javax.swing.JMenuBar
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JMenuBar getDebuggerFrameJMenuBar() {
    if (ivjDebuggerFrameJMenuBar == null) {
        try {
            ivjDebuggerFrameJMenuBar = new javax.swing.JMenuBar();
            ivjDebuggerFrameJMenuBar.setName("DebuggerFrameJMenuBar");
            ivjDebuggerFrameJMenuBar.add(getmnuFile());
            ivjDebuggerFrameJMenuBar.add(getmnuEditor());
            ivjDebuggerFrameJMenuBar.add(getmnuProgram());
            ivjDebuggerFrameJMenuBar.add(getmnuOptions());
            ivjDebuggerFrameJMenuBar.add(getmnuWindow());
            ivjDebuggerFrameJMenuBar.add(getmnuHelp());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjDebuggerFrameJMenuBar;
}
/**
 * Return the dskSubWindowArea property value.
 * @return javax.swing.JDesktopPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JDesktopPane getdskSubWindowArea() {
    if (ivjdskSubWindowArea == null) {
        try {
            ivjdskSubWindowArea = new javax.swing.JDesktopPane();
            ivjdskSubWindowArea.setName("dskSubWindowArea");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjdskSubWindowArea;
}

/**
 * Return the JFrameContentPane property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getJFrameContentPane() {
    if (ivjJFrameContentPane == null) {
        try {
            ivjJFrameContentPane = new javax.swing.JPanel();
            ivjJFrameContentPane.setName("JFrameContentPane");
            ivjJFrameContentPane.setLayout(new java.awt.BorderLayout());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjJFrameContentPane;
}

/**
 * Accessor for process table model
 */
public ProcessTableModel getProcessTableModel() {
   return processTableModel;
}

/**
 * Return the lstProcesses property value.
 * @return javax.swing.JTable
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTable getlstProcesses() {
    if (ivjlstProcesses == null) {
        try {
            ivjlstProcesses = new javax.swing.JTable();
            ivjlstProcesses.setName("lstProcesses");
            ivjlstProcesses.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjlstProcesses.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_OFF);
            ivjlstProcesses.setBounds(0, 0, 200, 200);
            // user code begin {1}
            ivjlstProcesses.setRowSelectionAllowed(true);
            ivjlstProcesses.setColumnSelectionAllowed(false);
            ivjlstProcesses.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            processTableModel = new ProcessTableModel(debugger.getInterpreter());
            ivjlstProcesses.setModel(processTableModel);
            ProcessCellRenderer renderer = new ProcessCellRenderer(debugger.getInterpreter());
            javax.swing.table.TableColumn column;
            for (int i = 0; i < 7; i++) {
               column = ivjlstProcesses.getColumnModel().getColumn(i);
               column.setCellRenderer(renderer);
               switch (i) {               
                    case 0: column.setHeaderValue(Config.PROC_PROCESS); column.setPreferredWidth(70); break;
                    case 1: column.setHeaderValue(Config.PROC_ACTIVE);  column.setPreferredWidth(50); break;
                    case 2: column.setHeaderValue(Config.PROC_SUSPEND); column.setPreferredWidth(60); break;
                    case 3: column.setHeaderValue(Config.PROC_FINISH);  column.setPreferredWidth(50); break;
                    case 4: column.setHeaderValue(Config.PROC_MONITOR); column.setPreferredWidth(60); break;
                    case 5: column.setHeaderValue(Config.PROC_PRIORITY);column.setPreferredWidth(30); break;
                    case 6: column.setHeaderValue(Config.PROC_ATOMIC);  column.setPreferredWidth(30); break;
                }
            }
             // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlstProcesses;
}

/**
 * Return the mncShowActive property value.
 * @return javax.swing.JCheckBoxMenuItem
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JCheckBoxMenuItem getmncShowActive() {
    if (ivjmncShowActive == null) {
        try {
            ivjmncShowActive = new javax.swing.JCheckBoxMenuItem();
            ivjmncShowActive.setName("mncShowActive");
            ivjmncShowActive.setText(Config.Active);
            ivjmncShowActive.setMnemonic(Config.ActiveMN);
            ivjmncShowActive.setState(Config.getBooleanProperty("SHOW_ACTIVE_DEFAULT"));
            setShowActiveWindow(Config.getBooleanProperty("SHOW_ACTIVE_DEFAULT")); 
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmncShowActive;
}

/**
 * Return the mncHistorySource property value.
 * @return javax.swing.JCheckBoxMenuItem
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JCheckBoxMenuItem getmncHistorySource() {
    if (ivjmncHistorySource == null) {
        try {
            ivjmncHistorySource = new javax.swing.JCheckBoxMenuItem();
            ivjmncHistorySource.setName("mncHistorySource");
            ivjmncHistorySource.setText(Config.HistorySource);
            ivjmncHistorySource.setMnemonic(Config.HistorySourceMN);
            ivjmncHistorySource.setState(Config.getBooleanProperty("SHOW_HISTORY_SOURCE"));
	    historySource = Config.getBooleanProperty("SHOW_HISTORY_SOURCE"); 
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmncHistorySource;
}

/**
 * Return the mncHistoryFile property value.
 * @return javax.swing.JCheckBoxMenuItem
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JCheckBoxMenuItem getmncHistoryFile() {
    if (ivjmncHistoryFile == null) {
        try {
            ivjmncHistoryFile = new javax.swing.JCheckBoxMenuItem();
            ivjmncHistoryFile.setName("mncHistoryFile");
            ivjmncHistoryFile.setText(Config.HistoryFile);
            ivjmncHistoryFile.setMnemonic(Config.HistoryFileMN);
            ivjmncHistoryFile.setState(Config.getBooleanProperty("WRITE_HISTORY_FILE"));
	    historyFile = Config.getBooleanProperty("WRITE_HISTORY_FILE"); 
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmncHistoryFile;
}

/**
 * Return the mncStopOnSwap property value.
 * @return javax.swing.JCheckBoxMenuItem
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JCheckBoxMenuItem getmncStopOnSwap() {
    if (ivjmncStopOnSwap == null) {
        try {
            ivjmncStopOnSwap = new javax.swing.JCheckBoxMenuItem();
            ivjmncStopOnSwap.setName("mncStopOnSwap");
            ivjmncStopOnSwap.setText(Config.Swap);
            ivjmncStopOnSwap.setMnemonic(Config.SwapMN);
            ivjmncStopOnSwap.setState(Config.getBooleanProperty("STOP_ON_SWAP_DEFAULT"));
            setStopOnSwap();
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmncStopOnSwap;
}
/**
 * Return the mnuProgram property value.
 * @return javax.swing.JMenu
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JMenu getmnuProgram() {
    if (ivjmnuProgram == null) {
        try {
            ivjmnuProgram = new javax.swing.JMenu();
            ivjmnuProgram.setName("mnuProgram");
            ivjmnuProgram.setText(Config.Program);
            ivjmnuProgram.setMnemonic(Config.ProgramMN);
            // user code begin {1}
            ivjmnuProgram.add(getbtnEdit());
            ivjmnuProgram.add(getbtnCompile());
            ivjmnuProgram.add(getbtnRun());
            ivjmnuProgram.addSeparator();
            ivjmnuProgram.add(getbtnGo());
            ivjmnuProgram.add(getbtnPause());
            ivjmnuProgram.addSeparator();
            ivjmnuProgram.add(getbtnSourceStep());
            ivjmnuProgram.add(getbtnPcodeStep());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuProgram;
}

private javax.swing.JMenu getmnuEditor() {
    if (ivjmnuEditor == null) {
        try {
            ivjmnuEditor = new javax.swing.JMenu();
            ivjmnuEditor.setName("mnuEditor");
            ivjmnuEditor.setText(Config.Editor);
            ivjmnuEditor.setMnemonic(Config.EditorMN);
            // user code begin {1}
            ivjmnuEditor.add(getbtnCopy());
            ivjmnuEditor.add(getbtnCut());
            ivjmnuEditor.add(getbtnPaste());
            ivjmnuEditor.addSeparator();
            ivjmnuEditor.add(getbtnFind());
            ivjmnuEditor.add(getbtnFindAgain());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuEditor;
}

/**
 * Return the JMenu1 property value.
 * @return javax.swing.JMenu
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JMenu getmnuFile() {
    if (ivjmnuFile == null) {
        try {
            ivjmnuFile = new javax.swing.JMenu();
            ivjmnuFile.setName("mnuFile");
            ivjmnuFile.setText(Config.File);
            ivjmnuFile.setMnemonic(Config.FileMN);
            // user code begin {1}
            ivjmnuFile.add(getbtnNew());
            ivjmnuFile.add(getbtnOpen());
            ivjmnuFile.add(getbtnSave());
            ivjmnuFile.add(getbtnSaveAs());
            ivjmnuFile.addSeparator();
            ivjmnuFile.add(getbtnExit());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuFile;
}
/**
 * Return the mnuOptions property value.
 * @return javax.swing.JMenu
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JMenu getmnuOptions() {
    if (ivjmnuOptions == null) {
        try {
            ivjmnuOptions = new javax.swing.JMenu();
            ivjmnuOptions.setName("mnuOptions");
            ivjmnuOptions.setText(Config.Options);
            ivjmnuOptions.setMnemonic(Config.OptionsMN);
            ivjmnuOptions.add(getmncStopOnSwap());
            ivjmnuOptions.add(getmncShowActive());
            ivjmnuOptions.add(getmncHistorySource());
            ivjmnuOptions.add(getmncHistoryFile());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuOptions;
}

/**
 * Return the mnuWindow property value.
 * @return javax.swing.JMenu
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JMenu getmnuWindow() {
    if (ivjmnuWindow == null) {
        try {
            ivjmnuWindow = new javax.swing.JMenu();
            ivjmnuWindow.setName("mnuWindow");
            ivjmnuWindow.setText(Config.Window);
            ivjmnuWindow.setMnemonic(Config.WindowMN);
            // user code begin {1}
            ivjmnuWindow.add(getbtnConsole());
            ivjmnuWindow.add(getbtnGlobals());
            ivjmnuWindow.add(getbtnHistory());
            ivjmnuWindow.add(getbtnLinda());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuWindow;
}

/**
 * Return the mnuHelp property value.
 * @return javax.swing.JMenu
 */
private javax.swing.JMenu getmnuHelp() {
    if (ivjmnuHelp == null) {
        try {
            ivjmnuHelp = new javax.swing.JMenu();
            ivjmnuHelp.setName("mnuHelp");
            ivjmnuHelp.setText(Config.Help);
            ivjmnuHelp.setMnemonic(Config.HelpMN);
            // user code begin {1}
            ivjmnuHelp.add(getbtnAbout());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjmnuHelp;
}

/**
 * Return the pnlProcessButton property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlProcessButton() {
    if (ivjpnlProcessButton == null) {
        try {
            ivjpnlProcessButton = new javax.swing.JPanel();
            ivjpnlProcessButton.setName("pnlProcessButton");
            ivjpnlProcessButton.setLayout(new java.awt.BorderLayout());
//            getpnlProcessButton().add(getbtnOpenProc(), "South");
            getpnlProcessButton().add(getbtnNextProc(), "East");
            getpnlProcessButton().add(getbtnPrevProc(), "West");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlProcessButton;
}

/**
 * Return the pnlProcesses property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlProcesses() {
    if (ivjpnlProcesses == null) {
        try {
            ivjpnlProcesses = new javax.swing.JPanel();
            ivjpnlProcesses.setName("pnlProcesses");
            ivjpnlProcesses.setBorder(new TitledBorder(Config.PROC_TITLE));
            ivjpnlProcesses.setLayout(new java.awt.BorderLayout());
            ivjpnlProcesses.setPreferredSize(new java.awt.Dimension(
	      Config.getIntegerProperty("PTAB_X"), Config.getIntegerProperty("PTAB_Y")));
//	    	Config.PROC_X, Config.PROC_Y));
            getpnlProcesses().add(getscpProcesses(), "Center");
            getpnlProcesses().add(getpnlProcessButton(), "South");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlProcesses;
}
/**
 * Return the scpProcesses property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpProcesses() {
    if (ivjscpProcesses == null) {
        try {
            ivjscpProcesses = new javax.swing.JScrollPane();
            ivjscpProcesses.setName("scpProcesses");
            ivjscpProcesses.setPreferredSize(new java.awt.Dimension(
	      	      Config.getIntegerProperty("PTAB_X"), Config.getIntegerProperty("PTAB_Y")));
	    // Config.PROC_X, Config.PROC_Y));
            getscpProcesses().setViewportView(getlstProcesses());
            getscpProcesses().setColumnHeaderView(ivjlstProcesses.getTableHeader());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpProcesses;
}
/**
 * Return the splMain property value.
 * @return javax.swing.JSplitPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JSplitPane getsplMain() {
    if (ivjsplMain == null) {
        try {
            ivjsplMain = new javax.swing.JSplitPane(javax.swing.JSplitPane.HORIZONTAL_SPLIT);
            ivjsplMain.setName("splMain");
            ivjsplMain.setDividerSize(8);
            getsplMain().add(getdskSubWindowArea(), "right");
            getsplMain().add(getpnlProcesses(), "left");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjsplMain;
}
/**
 * Return the tlbToolBar property value.
 * @return javax.swing.JToolBar
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JToolBar gettlbToolBar() {
    if (ivjtlbToolBar == null) {
        try {
            ivjtlbToolBar = new javax.swing.JToolBar();
            ivjtlbToolBar.setName("tlbToolBar");
            ivjtlbToolBar.setFloatable(false);
            // user code begin {1}
            ivjtlbToolBar.add(getbtnOpen());
            ivjtlbToolBar.add(getbtnSave());
            ivjtlbToolBar.addSeparator();
            ivjtlbToolBar.add(getbtnCopy());
            ivjtlbToolBar.add(getbtnCut());
            ivjtlbToolBar.add(getbtnPaste());
            ivjtlbToolBar.add(getbtnFind());
            ivjtlbToolBar.add(getbtnFindAgain());
            ivjtlbToolBar.addSeparator();
            ivjtlbToolBar.add(getbtnEdit());
            ivjtlbToolBar.add(getbtnCompile());
            ivjtlbToolBar.add(getbtnRun());
            ivjtlbToolBar.addSeparator();
            ivjtlbToolBar.add(getbtnGo());
            ivjtlbToolBar.add(getbtnPause());
            ivjtlbToolBar.addSeparator();
            ivjtlbToolBar.add(getbtnSourceStep());
            ivjtlbToolBar.add(getbtnPcodeStep());
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtlbToolBar;
}
/**
 * Gets WindowManger for this DebuggerFrame
 * @return WindowManager
 */
public WindowManager getWindowManager() {
    return windowManager;
}
/**
 * Called whenever the part throws an exception.
 * @param exception java.lang.Throwable
 */
private void handleException(java.lang.Throwable exception) {

    /* Uncomment the following lines to print uncaught exceptions to stdout */
     //System.out.println("--------- UNCAUGHT EXCEPTION ---------");
     //exception.printStackTrace(System.out);
    showError(exception);
}
/**
 * Initializes connections
 * @exception java.lang.Exception The exception description.
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void initConnections() throws java.lang.Exception {
    // user code begin {1}
    // user code end
    this.addWindowListener(ivjEventHandler);
    getlstProcesses().addMouseListener(ivjEventHandler);
    getmncStopOnSwap().addActionListener(ivjEventHandler);
    getmncShowActive().addActionListener(ivjEventHandler);
//    getbtnOpenProc().addActionListener(ivjEventHandler);
    getbtnNextProc().addActionListener(ivjEventHandler);
    getbtnPrevProc().addActionListener(ivjEventHandler);
    getmncHistorySource().addActionListener(ivjEventHandler);
    getmncHistoryFile().addActionListener(ivjEventHandler);
}

/**
 * Initialize the class.
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void initialize() {
    try {
        // user code begin {1}
        // user code end
        setName("DebuggerFrame");
        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle(Config.SOFTWARE_NAME);
        setSize(Config.getIntegerProperty("FRAME_X"), Config.getIntegerProperty("FRAME_Y"));
        setJMenuBar(getDebuggerFrameJMenuBar());
        setContentPane(getJFrameContentPane());
        initConnections();
    } catch (java.lang.Throwable ivjExc) {
        handleException(ivjExc);
    }
    // user code begin {2}
    // user code end
}

/**
 * Determines if Window for active process should be displayed
 * @return boolean
 */
public boolean isShowActiveWindow() {
    return showActiveWindow;
}

public boolean isHistorySource() {
	return historySource;
}

public boolean isHistoryFile() {
	return historyFile;
}

/**
 * main entrypoint - starts the part when it is run as an application
 * @param args java.lang.String[]
 */
public static void main(java.lang.String[] args) {
    try {
        DebuggerFrame aDebuggerFrame;
        aDebuggerFrame = new DebuggerFrame(new baci.gui.Debugger());
        aDebuggerFrame.addWindowListener(new java.awt.event.WindowAdapter() {
            public void WindowClosing(java.awt.event.WindowEvent e) {
                System.exit(0);
            };
        });
        aDebuggerFrame.show();
        java.awt.Insets insets = aDebuggerFrame.getInsets();
        aDebuggerFrame.setSize(aDebuggerFrame.getWidth() + insets.left + insets.right, aDebuggerFrame.getHeight() + insets.top + insets.bottom);
        aDebuggerFrame.setVisible(true);
    } catch (Throwable exception) {
        System.err.println("Exception occurred in main() of javax.swing.JFrame");
        exception.printStackTrace(System.out);
    }
}
/**
 * Opens a Window for the specified process
 * @param index
 */
public void openProcessWindow(int index) {
    if(index!=-1) {
      windowManager.showProcWindow(debugger.getInterpreter().getProcess(index), debugger);
      if (debugger.getInterpreter().getProcess(index).isActive())
	      debugger.getInterpreter().changeToProcess(index);
    }
}

/**
 * Change process and process window by delta = +/- 1.
 */
public void changeProcessWindow(int delta) {
    Interpreter i = debugger.getInterpreter();
    int next = i.nextProcess(delta);
    if (next != -1)
        windowManager.showProcWindow(i.getProcess(next), debugger);
}

/**
 * Called when a process is selected from the list
 * @param mouseEvent
 */
private void processSelected(MouseEvent mouseEvent) {
//  if(mouseEvent.getClickCount()==2) {
        openProcessWindow(getlstProcesses().rowAtPoint(mouseEvent.getPoint()));
//    }
}
/**
 * Sets if the Window for the active process should be displayed
 * @param newShowActiveWindow
 */
public void setShowActiveWindow(boolean newShowActiveWindow) {
    saveShowActiveWindow = showActiveWindow;
    showActiveWindow = newShowActiveWindow;
}

// Reset show active window flag during pause, etc.
public void resetShowActiveWindow() {
    showActiveWindow = saveShowActiveWindow;
}

/**
 * Sets if the interpreter should stop on process swaps
 */
public void setStopOnSwap() {
    getDebugger().getInterpreter().setStopOnSwap(getmncStopOnSwap().getState());
}
/**
 * Shows an error message
 * @param t
 */
public void showError(Throwable t) {
    String s = t.getMessage();
    JOptionPane.showMessageDialog(this, s==null ? t : (Object) s, "BACI error", JOptionPane.ERROR_MESSAGE);
    t.printStackTrace();
}

}
