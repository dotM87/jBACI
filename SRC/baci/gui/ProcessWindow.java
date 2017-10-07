package baci.gui;

import baci.event.*;
import baci.interpreter.*;
import baci.gui.treetable.*;
import baci.program.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
/**
 * Subwindow for process data
 * @author: David Strite
 */
public class ProcessWindow extends BaciWindow implements ProcessListener, ListSelectionListener {
    private baci.interpreter.BaciProcess process;
    private JPanel ivjJInternalFrameContentPane = null;
    private JSplitPane ivjsplCode = null;
    private JTabbedPane ivjtbpProcessData = null;
    private JPanel ivjpnlPcode = null;
    private JPanel ivjpnlSource = null;
    private JScrollPane ivjscpPcode = null;
    private JScrollPane ivjscpSource = null;
    private CodeTable ivjtblPcode = null;
    private CodeTable ivjtblSource = null;
    private JTextArea ivjtxaConsole = null;
    IvjEventHandler ivjEventHandler = new IvjEventHandler();
    private JButton ivjbtnAddPcodeBreakpoint = null;
    private JButton ivjbtnRemovePcodeBreakpoint = null;
    private JButton ivjbtnAddSourceBreakpoint = null;
    private JButton ivjbtnRemoveSourceBreakpoint = null;
    private JToolBar ivjtlbSource = null;
    private JSplitPane ivjsplDebug = null;
    private JScrollPane ivjscpConsole = null;
    private JScrollPane ivjscpVariables = null;
    private JPanel ivjpnlVariables = null;
    private JToolBar ivjtlbPcodeBreakpoints = null;
    private JScrollPane ivjscpStack = null;
    private JSplitPane ivjsplDetails = null;
    private JTable ivjtblStack = null;
    private JPanel ivjpnlStack = null;
    private JPanel ivjpnlOtherDetails = null;
    private JLabel ivjlblBottom = null;
    private JTextField ivjtxtBottom = null;
    private JLabel ivjlblTop = null;
    private JTextField ivjtxtTop = null;
    private JLabel ivjlblActive = null;
    private JTextField ivjtxtActive = null;
    private JLabel ivjlblFinished = null;
    private JTextField ivjtxtFinished = null;
    private JLabel ivjlblSuspend = null;
    private JTextField ivjtxtSuspend = null;
    private JLabel ivjlblMonitor = null;
    private JTextField ivjtxtMonitor = null;
    private JLabel ivjlblPriority = null;
    private JTextField ivjtxtPriority = null;
    private JLabel ivjlblAtomic = null;
    private JTextField ivjtxtAtomic = null;

class IvjEventHandler implements java.awt.event.ActionListener, java.awt.event.MouseListener {
        public void actionPerformed(java.awt.event.ActionEvent e) {
            if (e.getSource() == ProcessWindow.this.getbtnAddPcodeBreakpoint()) 
                connEtoC1(e);
            if (e.getSource() == ProcessWindow.this.getbtnRemovePcodeBreakpoint()) 
                connEtoC2(e);
            if (e.getSource() == ProcessWindow.this.getbtnAddSourceBreakpoint()) 
                connEtoC3(e);
            if (e.getSource() == ProcessWindow.this.getbtnRemoveSourceBreakpoint()) 
                connEtoC4(e);
        };
        public void mouseClicked(java.awt.event.MouseEvent e) {};
        public void mouseEntered(java.awt.event.MouseEvent e) {};
        public void mouseExited(java.awt.event.MouseEvent e) {};
        public void mousePressed(java.awt.event.MouseEvent e) {
            if (e.getSource() == ProcessWindow.this.gettblPcode()) 
                connEtoC5(e);
            if (e.getSource() == ProcessWindow.this.gettblSource()) 
                connEtoC6(e);
        };
        public void mouseReleased(java.awt.event.MouseEvent e) {};
    };
/**
 * ProcessWindow constructor comment.
 */
public ProcessWindow() {
    super();
    initialize();
}
/**
 * Construct new ProcessWindow with specified Process and Debugger
 * @param proc
 * @param debugger
 */
public ProcessWindow(BaciProcess proc,Debugger debugger) {
    super(debugger);
    process = proc;
    process.addListener(this);
    initialize();
}
/**
 * Adds a pcode break point
 */
private void addPcodeBreakpoint() {
    PcodeModel model = (PcodeModel)gettblPcode().getModel();
    int selectedRow = gettblPcode().getSelectedRow();

    BreakpointManager manager = getDebugger().getInterpreter().getBreakpointManager();

    manager.addBreakpoint(model.getBeginIndex()+selectedRow);
    gettblPcode().getSelectionModel().setSelectionInterval(selectedRow,selectedRow);
}
/**
 * Adds a source break point
 */
private void addSourceBreakpoint() {
    SourceModel model = (SourceModel)gettblSource().getModel();
    int selectedRow = gettblSource().getSelectedRow();

    BreakpointManager manager = getDebugger().getInterpreter().getBreakpointManager();

    PcodeInstruction pcode = model.getProcess().getProgram().getPcode(model.getProcess().getCurrentPcode());
    manager.addBreakpoint(pcode.getFileIndex(),selectedRow);
    gettblSource().getSelectionModel().setSelectionInterval(selectedRow,selectedRow);
}
/**
 * connEtoC1:  (btnAddPcodeBreakPoint.action.actionPerformed(java.awt.event.ActionEvent) --> ProcessWindow.addPcodeBreakpoint(Ljava.awt.event.ActionEvent;)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC1(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.addPcodeBreakpoint();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC2:  (btnRemovePcodeBreakpoint.action.actionPerformed(java.awt.event.ActionEvent) --> ProcessWindow.removePcodeBreakpoint(Ljava.awt.event.ActionEvent;)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC2(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.removePcodeBreakpoint();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC3:  (btnAddSourceBreakpoint.action.actionPerformed(java.awt.event.ActionEvent) --> ProcessWindow.addSourceBreakpoint(Ljava.awt.event.ActionEvent;)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC3(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.addSourceBreakpoint();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC4:  (btnRemoveSourceBreakpoint.action.actionPerformed(java.awt.event.ActionEvent) --> ProcessWindow.removeSourceBreakpoint(Ljava.awt.event.ActionEvent;)V)
 * @param arg1 java.awt.event.ActionEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC4(java.awt.event.ActionEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.removeSourceBreakpoint();
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC5:  (tblPcode.mouse.mousePressed(java.awt.event.MouseEvent) --> ProcessWindow.pcodeTableClick()V)
 * @param arg1 java.awt.event.MouseEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC5(java.awt.event.MouseEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.pcodeTablePressed(arg1);
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * connEtoC6:  (tblSource.mouse.mouseClicked(java.awt.event.MouseEvent) --> ProcessWindow.sourceTableClick(Ljava.awt.event.MouseEvent;)V)
 * @param arg1 java.awt.event.MouseEvent
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void connEtoC6(java.awt.event.MouseEvent arg1) {
    try {
        // user code begin {1}
        // user code end
        this.pcodeTablePressed(arg1);
        // user code begin {2}
        // user code end
    } catch (java.lang.Throwable ivjExc) {
        // user code begin {3}
        // user code end
        handleException(ivjExc);
    }
}
/**
 * Return the btnAddPcodeBreakPoint property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnAddPcodeBreakpoint() {
    if (ivjbtnAddPcodeBreakpoint == null) {
        try {
            ivjbtnAddPcodeBreakpoint = new javax.swing.JButton();
            ivjbtnAddPcodeBreakpoint.setName("btnAddPcodeBreakpoint");
            ivjbtnAddPcodeBreakpoint.setText(Config.AddPcodeBreak);
            ivjbtnAddPcodeBreakpoint.setMnemonic(Config.AddPcodeBreakMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnAddPcodeBreakpoint;
}
/**
 * Return the btnAddSourceBreakpoint property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnAddSourceBreakpoint() {
    if (ivjbtnAddSourceBreakpoint == null) {
        try {
            ivjbtnAddSourceBreakpoint = new javax.swing.JButton();
            ivjbtnAddSourceBreakpoint.setName("btnAddSourceBreakpoint");
            ivjbtnAddSourceBreakpoint.setText(Config.AddSourceBreak);
            ivjbtnAddSourceBreakpoint.setMnemonic(Config.AddSourceBreakMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnAddSourceBreakpoint;
}
/**
 * Return the btnRemovePcodeBreakpoint property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnRemovePcodeBreakpoint() {
    if (ivjbtnRemovePcodeBreakpoint == null) {
        try {
            ivjbtnRemovePcodeBreakpoint = new javax.swing.JButton();
            ivjbtnRemovePcodeBreakpoint.setName("btnRemovePcodeBreakpoint");
            ivjbtnRemovePcodeBreakpoint.setText(Config.RemovePcodeBreak);
            ivjbtnRemovePcodeBreakpoint.setMnemonic(Config.RemovePcodeBreakMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnRemovePcodeBreakpoint;
}
/**
 * Return the btnRemoveSourceBreakpoint property value.
 * @return javax.swing.JButton
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JButton getbtnRemoveSourceBreakpoint() {
    if (ivjbtnRemoveSourceBreakpoint == null) {
        try {
            ivjbtnRemoveSourceBreakpoint = new javax.swing.JButton();
            ivjbtnRemoveSourceBreakpoint.setName("btnRemoveSourceBreakpoint");
            ivjbtnRemoveSourceBreakpoint.setText(Config.RemoveSourceBreak);
            ivjbtnRemoveSourceBreakpoint.setMnemonic(Config.RemoveSourceBreakMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjbtnRemoveSourceBreakpoint;
}
/**
 * Return the JInternalFrameContentPane property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getJInternalFrameContentPane() {
    if (ivjJInternalFrameContentPane == null) {
        try {
            ivjJInternalFrameContentPane = new javax.swing.JPanel();
            ivjJInternalFrameContentPane.setName("JInternalFrameContentPane");
            ivjJInternalFrameContentPane.setLayout(new java.awt.BorderLayout());
            getJInternalFrameContentPane().add(gettbpProcessData(), "Center");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjJInternalFrameContentPane;
}
/**
 * Return the lblActive property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblActive() {
    if (ivjlblActive == null) {
        try {
            ivjlblActive = new javax.swing.JLabel();
            ivjlblActive.setName("lblActive");
            ivjlblActive.setText(Config.PW_ACTIVE);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblActive;
}
/**
 * Return the lblAtomic property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblAtomic() {
    if (ivjlblAtomic == null) {
        try {
            ivjlblAtomic = new javax.swing.JLabel();
            ivjlblAtomic.setName("lblAtomic");
            ivjlblAtomic.setText(Config.PW_ATOMIC);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblAtomic;
}
/**
 * Return the lblBottom property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblBottom() {
    if (ivjlblBottom == null) {
        try {
            ivjlblBottom = new javax.swing.JLabel();
            ivjlblBottom.setName("lblBottom");
            ivjlblBottom.setText(Config.PW_BOTTOM);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblBottom;
}
/**
 * Return the lblFinished property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblFinished() {
    if (ivjlblFinished == null) {
        try {
            ivjlblFinished = new javax.swing.JLabel();
            ivjlblFinished.setName("lblFinished");
            ivjlblFinished.setText(Config.PW_FINISHED);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblFinished;
}
/**
 * Return the lblMonitor property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblMonitor() {
    if (ivjlblMonitor == null) {
        try {
            ivjlblMonitor = new javax.swing.JLabel();
            ivjlblMonitor.setName("lblMonitor");
            ivjlblMonitor.setText(Config.PW_MONITOR);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblMonitor;
}
/**
 * Return the lblPriority property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblPriority() {
    if (ivjlblPriority == null) {
        try {
            ivjlblPriority = new javax.swing.JLabel();
            ivjlblPriority.setName("lblPriority");
            ivjlblPriority.setText(Config.PW_PRIORITY);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblPriority;
}
/**
 * Return the lblSuspend property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblSuspend() {
    if (ivjlblSuspend == null) {
        try {
            ivjlblSuspend = new javax.swing.JLabel();
            ivjlblSuspend.setName("lblSuspend");
            ivjlblSuspend.setText(Config.PW_SUSPEND);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblSuspend;
}
/**
 * Return the lblTop property value.
 * @return javax.swing.JLabel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JLabel getlblTop() {
    if (ivjlblTop == null) {
        try {
            ivjlblTop = new javax.swing.JLabel();
            ivjlblTop.setName("lblTop");
            ivjlblTop.setText(Config.PW_TOP);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjlblTop;
}
/**
 * Return the pnlOtherDetails property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlOtherDetails() {
    if (ivjpnlOtherDetails == null) {
        try {
            ivjpnlOtherDetails = new javax.swing.JPanel();
            ivjpnlOtherDetails.setName("pnlOtherDetails");
            ivjpnlOtherDetails.setBorder(new TitledBorder(Config.PW_TITLE));
            ivjpnlOtherDetails.setLayout(new java.awt.GridBagLayout());

            java.awt.GridBagConstraints constraintslblBottom = new java.awt.GridBagConstraints();
            constraintslblBottom.gridx = 0; constraintslblBottom.gridy = 0;
            constraintslblBottom.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblBottom.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblBottom(), constraintslblBottom);

            java.awt.GridBagConstraints constraintstxtBottom = new java.awt.GridBagConstraints();
            constraintstxtBottom.gridx = 1; constraintstxtBottom.gridy = 0;
            constraintstxtBottom.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtBottom.weightx = 1.0;
            constraintstxtBottom.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtBottom(), constraintstxtBottom);

            java.awt.GridBagConstraints constraintslblTop = new java.awt.GridBagConstraints();
            constraintslblTop.gridx = 0; constraintslblTop.gridy = 1;
            constraintslblTop.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblTop.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblTop(), constraintslblTop);

            java.awt.GridBagConstraints constraintstxtTop = new java.awt.GridBagConstraints();
            constraintstxtTop.gridx = 1; constraintstxtTop.gridy = 1;
            constraintstxtTop.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtTop.weightx = 1.0;
            constraintstxtTop.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtTop(), constraintstxtTop);

            java.awt.GridBagConstraints constraintslblActive = new java.awt.GridBagConstraints();
            constraintslblActive.gridx = 0; constraintslblActive.gridy = 2;
            constraintslblActive.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblActive.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblActive(), constraintslblActive);

            java.awt.GridBagConstraints constraintstxtActive = new java.awt.GridBagConstraints();
            constraintstxtActive.gridx = 1; constraintstxtActive.gridy = 2;
            constraintstxtActive.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtActive.weightx = 1.0;
            constraintstxtActive.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtActive(), constraintstxtActive);

            java.awt.GridBagConstraints constraintslblFinished = new java.awt.GridBagConstraints();
            constraintslblFinished.gridx = 0; constraintslblFinished.gridy = 3;
            constraintslblFinished.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblFinished.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblFinished(), constraintslblFinished);

            java.awt.GridBagConstraints constraintstxtFinished = new java.awt.GridBagConstraints();
            constraintstxtFinished.gridx = 1; constraintstxtFinished.gridy = 3;
            constraintstxtFinished.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtFinished.weightx = 1.0;
            constraintstxtFinished.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtFinished(), constraintstxtFinished);

            java.awt.GridBagConstraints constraintslblSuspend = new java.awt.GridBagConstraints();
            constraintslblSuspend.gridx = 0; constraintslblSuspend.gridy = 4;
            constraintslblSuspend.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblSuspend.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblSuspend(), constraintslblSuspend);

            java.awt.GridBagConstraints constraintstxtSuspend = new java.awt.GridBagConstraints();
            constraintstxtSuspend.gridx = 1; constraintstxtSuspend.gridy = 4;
            constraintstxtSuspend.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtSuspend.weightx = 1.0;
            constraintstxtSuspend.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtSuspend(), constraintstxtSuspend);

            java.awt.GridBagConstraints constraintslblMonitor = new java.awt.GridBagConstraints();
            constraintslblMonitor.gridx = 0; constraintslblMonitor.gridy = 5;
            constraintslblMonitor.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblMonitor.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblMonitor(), constraintslblMonitor);

            java.awt.GridBagConstraints constraintstxtMonitor = new java.awt.GridBagConstraints();
            constraintstxtMonitor.gridx = 1; constraintstxtMonitor.gridy = 5;
            constraintstxtMonitor.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtMonitor.weightx = 1.0;
            constraintstxtMonitor.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtMonitor(), constraintstxtMonitor);

            java.awt.GridBagConstraints constraintslblPriority = new java.awt.GridBagConstraints();
            constraintslblPriority.gridx = 0; constraintslblPriority.gridy = 6;
            constraintslblPriority.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblPriority.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblPriority(), constraintslblPriority);

            java.awt.GridBagConstraints constraintstxtPriority = new java.awt.GridBagConstraints();
            constraintstxtPriority.gridx = 1; constraintstxtPriority.gridy = 6;
            constraintstxtPriority.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtPriority.weightx = 1.0;
            constraintstxtPriority.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtPriority(), constraintstxtPriority);

            java.awt.GridBagConstraints constraintslblAtomic = new java.awt.GridBagConstraints();
            constraintslblAtomic.gridx = 0; constraintslblAtomic.gridy = 7;
            constraintslblAtomic.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintslblAtomic.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(getlblAtomic(), constraintslblAtomic);

            java.awt.GridBagConstraints constraintstxtAtomic = new java.awt.GridBagConstraints();
            constraintstxtAtomic.gridx = 1; constraintstxtAtomic.gridy = 7;
            constraintstxtAtomic.anchor = java.awt.GridBagConstraints.NORTHWEST;
            constraintstxtAtomic.weightx = 1.0;
            constraintstxtAtomic.weighty = 1.0;
            constraintstxtAtomic.insets = new java.awt.Insets(4, 4, 4, 4);
            getpnlOtherDetails().add(gettxtAtomic(), constraintstxtAtomic);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlOtherDetails;
}
/**
 * Return the pnlPcode property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlPcode() {
    if (ivjpnlPcode == null) {
        try {
            ivjpnlPcode = new javax.swing.JPanel();
            ivjpnlPcode.setName("pnlPcode");
            ivjpnlPcode.setPreferredSize(new java.awt.Dimension(50, 428));
            ivjpnlPcode.setBorder(new TitledBorder(Config.PW_PCODE));
            ivjpnlPcode.setLayout(new java.awt.BorderLayout());
            getpnlPcode().add(getscpPcode(), "Center");
            getpnlPcode().add(gettlbPcodeBreakpoints(), "North");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlPcode;
}
/**
 * Return the pnlSource property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlSource() {
    if (ivjpnlSource == null) {
        try {
            ivjpnlSource = new javax.swing.JPanel();
            ivjpnlSource.setName("pnlSource");
            ivjpnlSource.setPreferredSize(new java.awt.Dimension(300, 428));
            ivjpnlSource.setBorder(new TitledBorder(Config.PW_SOURCE));
            ivjpnlSource.setLayout(new java.awt.BorderLayout());
            getpnlSource().add(getscpSource(), "Center");
            getpnlSource().add(gettlbSource(), "North");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlSource;
}
/**
 * Return the pnlStack property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlStack() {
    if (ivjpnlStack == null) {
        try {
            ivjpnlStack = new javax.swing.JPanel();
            ivjpnlStack.setName("pnlStack");
            ivjpnlStack.setBorder(new TitledBorder(Config.PW_STACK));
            ivjpnlStack.setLayout(new java.awt.BorderLayout());
            getpnlStack().add(getscpStack(), "Center");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlStack;
}
/**
 * Return the pnlVariables property value.
 * @return javax.swing.JPanel
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JPanel getpnlVariables() {
    if (ivjpnlVariables == null) {
        try {
            ivjpnlVariables = new javax.swing.JPanel();
            ivjpnlVariables.setName("pnlVariables");
            ivjpnlVariables.setBorder(new TitledBorder(Config.PW_VARIABLES));
            ivjpnlVariables.setLayout(new java.awt.BorderLayout());
            getpnlVariables().add(getscpVariables(), "Center");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjpnlVariables;
}
/**
 * Get process this window is for
 * @return BaciProcess
 */
public BaciProcess getProcess() {
    return process;
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
 * Return the scpPcode property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpPcode() {
    if (ivjscpPcode == null) {
        try {
            ivjscpPcode = new javax.swing.JScrollPane();
            ivjscpPcode.setName("scpPcode");
            ivjscpPcode.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
            ivjscpPcode.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            getscpPcode().setViewportView(gettblPcode());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpPcode;
}
/**
 * Return the scpSource property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpSource() {
    if (ivjscpSource == null) {
        try {
            ivjscpSource = new javax.swing.JScrollPane();
            ivjscpSource.setName("scpSource");
            ivjscpSource.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
            ivjscpSource.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            getscpSource().setViewportView(gettblSource());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpSource;
}
/**
 * Return the scpStack property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpStack() {
    if (ivjscpStack == null) {
        try {
            ivjscpStack = new javax.swing.JScrollPane();
            ivjscpStack.setName("scpStack");
            ivjscpStack.setVerticalScrollBarPolicy(javax.swing.JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
            ivjscpStack.setHorizontalScrollBarPolicy(javax.swing.JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            ivjscpStack.setPreferredSize(new java.awt.Dimension(175, 403));
            getscpStack().setViewportView(gettblStack());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjscpStack;
}
/**
 * Return the scpVariables property value.
 * @return javax.swing.JScrollPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JScrollPane getscpVariables() {
    if (ivjscpVariables == null) {
        try {
            ivjscpVariables = new javax.swing.JScrollPane();
            ivjscpVariables.setName("scpVariables");
            // user code begin {1}
            if(process!=null) {
                JTreeTable treeTable = new JTreeTable(process.getVariableModel());
		treeTable.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
			Config.getIntegerProperty("TABLE_FONT_SIZE")));
                ivjscpVariables.setViewportView(treeTable);
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
    return ivjscpVariables;
}
/**
 * Return the splCode property value.
 * @return javax.swing.JSplitPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JSplitPane getsplCode() {
    if (ivjsplCode == null) {
        try {
            ivjsplCode = new javax.swing.JSplitPane(javax.swing.JSplitPane.HORIZONTAL_SPLIT);
            ivjsplCode.setName("splCode");
            ivjsplCode.setOneTouchExpandable(true);
            getsplCode().add(getpnlPcode(), "right");
            getsplCode().add(getpnlSource(), "left");
	    getsplCode().setDividerLocation(Config.getIntegerProperty("PROCESS_X"));
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjsplCode;
}
/**
 * Return the splDebug property value.
 * @return javax.swing.JSplitPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JSplitPane getsplDebug() {
    if (ivjsplDebug == null) {
        try {
            ivjsplDebug = new javax.swing.JSplitPane(javax.swing.JSplitPane.VERTICAL_SPLIT);
            ivjsplDebug.setName("splDebug");
            ivjsplDebug.setDividerLocation(150);
            getsplDebug().add(getsplCode(), "top");
            getsplDebug().add(getpnlVariables(), "bottom");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjsplDebug;
}
/**
 * Return the splDetails property value.
 * @return javax.swing.JSplitPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JSplitPane getsplDetails() {
    if (ivjsplDetails == null) {
        try {
            ivjsplDetails = new javax.swing.JSplitPane(javax.swing.JSplitPane.HORIZONTAL_SPLIT);
            ivjsplDetails.setName("splDetails");
            ivjsplDetails.setDividerLocation(150);
            getsplDetails().add(getpnlStack(), "left");
            getsplDetails().add(getpnlOtherDetails(), "right");
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjsplDetails;
}
/**
 * Return the tblPcode property value.
 * @return baci.gui.CodeTable
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private CodeTable gettblPcode() {
    if (ivjtblPcode == null) {
        try {
            ivjtblPcode = new baci.gui.CodeTable();
            ivjtblPcode.setName("tblPcode");
            ivjtblPcode.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("PCODE_FONT_SIZE")));
            getscpPcode().setColumnHeaderView(ivjtblPcode.getTableHeader());
            getscpPcode().getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
            ivjtblPcode.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_OFF);
            ivjtblPcode.setIntercellSpacing(new java.awt.Dimension(0, 0));
            ivjtblPcode.setBounds(0, 0, 200, 200);
            ivjtblPcode.setShowHorizontalLines(false);
            // user code begin {1}
            ivjtblPcode.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            if(process!=null) {
                PcodeModel model = process.getPcodeModel();
                ivjtblPcode.setModel(model);

                TableColumn column;
                column = ivjtblPcode.getColumnModel().getColumn(0);
                column.setPreferredWidth(20);
                column.setMinWidth(20);
                column.setMaxWidth(20);
                column.setCellRenderer(new StatusCellRenderer());
                column.setHeaderValue(null);

                column = ivjtblPcode.getColumnModel().getColumn(1);
                column.setCellRenderer(new CodeCellRenderer(false));
                column.setPreferredWidth(model.getMaxWidth(1,ivjtblPcode.getFontMetrics(ivjtblPcode.getFont())));
                column.setHeaderValue(null);

                getDebugger().getInterpreter().getBreakpointManager().addBreakpointlListener(model);
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtblPcode;
}
/**
 * Return the tblSource property value.
 * @return baci.gui.CodeTable
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private CodeTable gettblSource() {
    if (ivjtblSource == null) {
        try {
            ivjtblSource = new baci.gui.CodeTable();
            ivjtblSource.setName("tblSource");
            ivjtblSource.setFont(new java.awt.Font(Config.SOURCE_FONT_FAMILY, Config.SOURCE_FONT_STYLE, 
	    				   Config.getIntegerProperty("SOURCE_FONT_SIZE")));
            getscpSource().setColumnHeaderView(ivjtblSource.getTableHeader());
            getscpPcode().getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
            ivjtblSource.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_OFF);
            ivjtblSource.setIntercellSpacing(new java.awt.Dimension(0, 0));
            ivjtblSource.setBounds(0, 0, 200, 200);
            ivjtblSource.setShowHorizontalLines(false);
            // user code begin {1}
            ivjtblSource.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            if(process!=null) {
                SourceModel model = process.getSourceModel();
                ivjtblSource.setModel(model);
                ivjtblSource.getSelectionModel().addListSelectionListener(this);

                TableColumn column;
                column = ivjtblSource.getColumnModel().getColumn(0);
                column.setPreferredWidth(20);
                column.setCellRenderer(new StatusCellRenderer());
                column.setHeaderValue(null);

                column = ivjtblSource.getColumnModel().getColumn(1);
                column.setCellRenderer(new CodeCellRenderer(true));
                column.setPreferredWidth(model.getMaxWidth(1,ivjtblSource.getFontMetrics(ivjtblSource.getFont())));
                column.setHeaderValue(null);

                getDebugger().getInterpreter().getBreakpointManager().addBreakpointlListener(model);
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtblSource;
}

/**
 * Return the tblStack property value.
 * @return javax.swing.JTable
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTable gettblStack() {
    if (ivjtblStack == null) {
        try {
            ivjtblStack = new javax.swing.JTable();
            ivjtblStack.setName("tblStack");
            ivjtblStack.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            getscpStack().setColumnHeaderView(ivjtblStack.getTableHeader());
            getscpPcode().getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
            ivjtblStack.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_OFF);
            ivjtblStack.setBounds(0, 0, 200, 200);
            // user code begin {1}
            if(process!=null) {
                ivjtblStack.setModel(new StackTableModel(process));
                TableColumn column;

                column = ivjtblStack.getColumnModel().getColumn(0);
                column.setPreferredWidth(40);

                column = ivjtblStack.getColumnModel().getColumn(1);
                column.setPreferredWidth(100);

                ivjtblStack.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtblStack;
}
/**
 * Return the tbpProcessData property value.
 * @return javax.swing.JTabbedPane
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTabbedPane gettbpProcessData() {
    if (ivjtbpProcessData == null) {
        try {
            ivjtbpProcessData = new javax.swing.JTabbedPane();
            ivjtbpProcessData.setName("tbpProcessData");
            ivjtbpProcessData.insertTab(Config.CodeTab, null, getsplDebug(), null, 0);
            ivjtbpProcessData.insertTab(Config.ConsoleTab, null, getscpConsole(), null, 1);
            ivjtbpProcessData.insertTab(Config.DetailsTab, null, getsplDetails(), null, 2);
            ivjtbpProcessData.setMnemonicAt(0, Config.CodeTabMN);
            ivjtbpProcessData.setMnemonicAt(1, Config.ConsoleTabMN);
            ivjtbpProcessData.setMnemonicAt(2, Config.DetailsTabMN);
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtbpProcessData;
}
/**
 * Return the tblPcodeBreakpoints property value.
 * @return javax.swing.JToolBar
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JToolBar gettlbPcodeBreakpoints() {
    if (ivjtlbPcodeBreakpoints == null) {
        try {
            ivjtlbPcodeBreakpoints = new javax.swing.JToolBar();
            ivjtlbPcodeBreakpoints.setName("tlbPcodeBreakpoints");
            ivjtlbPcodeBreakpoints.setFloatable(false);
            gettlbPcodeBreakpoints().add(getbtnAddPcodeBreakpoint(), getbtnAddPcodeBreakpoint().getName());
            gettlbPcodeBreakpoints().add(getbtnRemovePcodeBreakpoint(), getbtnRemovePcodeBreakpoint().getName());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtlbPcodeBreakpoints;
}
/**
 * Return the tlbSource property value.
 * @return javax.swing.JToolBar
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JToolBar gettlbSource() {
    if (ivjtlbSource == null) {
        try {
            ivjtlbSource = new javax.swing.JToolBar();
            ivjtlbSource.setName("tlbSource");
            ivjtlbSource.setFloatable(false);
            gettlbSource().add(getbtnAddSourceBreakpoint(), getbtnAddSourceBreakpoint().getName());
            gettlbSource().add(getbtnRemoveSourceBreakpoint(), getbtnRemoveSourceBreakpoint().getName());
            // user code begin {1}
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtlbSource;
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
            ivjtxaConsole.setBounds(0, 0, 403, 229);
            ivjtxaConsole.setEditable(false);
            // user code begin {1}
            if(process!=null) {
                ivjtxaConsole.setDocument(process.getConsole());
            }
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
 * Return the JTextField property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtActive() {
    if (ivjtxtActive == null) {
        try {
            ivjtxtActive = new javax.swing.JTextField();
            ivjtxtActive.setName("txtActive");
            ivjtxtActive.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtActive.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtActive.setEditable(false);
            ivjtxtActive.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtActive.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtActive.setText(String.valueOf(process.isActive()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtActive;
}
/**
 * Return the txtAtomic property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtAtomic() {
    if (ivjtxtAtomic == null) {
        try {
            ivjtxtAtomic = new javax.swing.JTextField();
            ivjtxtAtomic.setName("txtAtomic");
            ivjtxtAtomic.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtAtomic.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtAtomic.setEditable(false);
            ivjtxtAtomic.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtAtomic.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtAtomic.setText(String.valueOf(process.getAtomicCount()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtAtomic;
}
/**
 * Return the txtBottom property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtBottom() {
    if (ivjtxtBottom == null) {
        try {
            ivjtxtBottom = new javax.swing.JTextField();
            ivjtxtBottom.setName("txtBottom");
            ivjtxtBottom.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtBottom.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtBottom.setEditable(false);
            ivjtxtBottom.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtBottom.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtBottom.setText(String.valueOf(process.getBottom()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtBottom;
}
/**
 * Return the txtFinished property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtFinished() {
    if (ivjtxtFinished == null) {
        try {
            ivjtxtFinished = new javax.swing.JTextField();
            ivjtxtFinished.setName("txtFinished");
            ivjtxtFinished.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtFinished.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtFinished.setEditable(false);
            ivjtxtFinished.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtFinished.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtFinished.setText(String.valueOf(process.isFinished()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtFinished;
}
/**
 * Return the txtMonitor property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtMonitor() {
    if (ivjtxtMonitor == null) {
        try {
            ivjtxtMonitor = new javax.swing.JTextField();
            ivjtxtMonitor.setName("txtMonitor");
            ivjtxtMonitor.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtMonitor.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtMonitor.setEditable(false);
            ivjtxtMonitor.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtMonitor.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtMonitor.setText(process.getMonitor()==null?"":process.getMonitor().toString());
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtMonitor;
}
/**
 * Return the txtPriority property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtPriority() {
    if (ivjtxtPriority == null) {
        try {
            ivjtxtPriority = new javax.swing.JTextField();
            ivjtxtPriority.setName("txtPriority");
            ivjtxtPriority.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtPriority.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtPriority.setEditable(false);
            ivjtxtPriority.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtPriority.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtPriority.setText(String.valueOf(process.getPriority()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtPriority;
}
/**
 * Return the txtSuspend property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtSuspend() {
    if (ivjtxtSuspend == null) {
        try {
            ivjtxtSuspend = new javax.swing.JTextField();
            ivjtxtSuspend.setName("txtSuspend");
            ivjtxtSuspend.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtSuspend.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtSuspend.setEditable(false);
            ivjtxtSuspend.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtSuspend.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtSuspend.setText(process.getSuspend()==null?"":process.getSuspend().toString());
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtSuspend;
}
/**
 * Return the txtTop property value.
 * @return javax.swing.JTextField
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private javax.swing.JTextField gettxtTop() {
    if (ivjtxtTop == null) {
        try {
            ivjtxtTop = new javax.swing.JTextField();
            ivjtxtTop.setName("txtTop");
            ivjtxtTop.setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	    		   Config.getIntegerProperty("TABLE_FONT_SIZE")));
            ivjtxtTop.setPreferredSize(new java.awt.Dimension(100, 20));
            ivjtxtTop.setEditable(false);
            ivjtxtTop.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
            ivjtxtTop.setMinimumSize(new java.awt.Dimension(100, 20));
            // user code begin {1}
            if(process!=null) {
                ivjtxtTop.setText(String.valueOf(process.getTop()));
            }
            // user code end
        } catch (java.lang.Throwable ivjExc) {
            // user code begin {2}
            // user code end
            handleException(ivjExc);
        }
    }
    return ivjtxtTop;
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
 * Initializes connections
 * @exception java.lang.Exception The exception description.
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void initConnections() throws java.lang.Exception {
    // user code begin {1}
    // user code end
    getbtnAddPcodeBreakpoint().addActionListener(ivjEventHandler);
    getbtnRemovePcodeBreakpoint().addActionListener(ivjEventHandler);
    getbtnAddSourceBreakpoint().addActionListener(ivjEventHandler);
    getbtnRemoveSourceBreakpoint().addActionListener(ivjEventHandler);
    gettblPcode().addMouseListener(ivjEventHandler);
    gettblSource().addMouseListener(ivjEventHandler);
}

/**
 * Initialize the class.
 */
/* WARNING: THIS METHOD WILL BE REGENERATED. */
private void initialize() {
    try {
        // user code begin {1}
        // user code end
        setName("ProcessWindow");
        setPreferredSize(new java.awt.Dimension(600, 500));
	setSize(Config.getIntegerProperty("PROCESS_X"), Config.getIntegerProperty("PROCESS_Y"));
        setContentPane(getJInternalFrameContentPane());
	int procNum = process.getIndex(); 
        setLocation((procNum % 3) * Config.getIntegerProperty("PROCESS_X"), (procNum / 3) * Config.OFF_Y);
        initConnections();
    } catch (java.lang.Throwable ivjExc) {
        handleException(ivjExc);
    }
    // user code begin {2}
    if(process!=null) {
        setTitle(Config.PW_PROCESS + " " + process.getName());
    }
    // user code end
}
/**
 * main entrypoint - starts the part when it is run as an application
c * @param args java.lang.String[]
 */
public static void main(java.lang.String[] args) {
    try {
        JFrame frame = new javax.swing.JFrame();
        ProcessWindow aProcessWindow;
        aProcessWindow = new ProcessWindow();
        frame.setContentPane(aProcessWindow);
        frame.setSize(aProcessWindow.getSize());
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
        System.err.println("Exception occurred in main() of javax.swing.JInternalFrame");
        exception.printStackTrace(System.out);
    }
}
/**
 * Called when mouse is pressed on pcode table
 */
public void pcodeTablePressed(MouseEvent mouseEvent) {
    CodeTable table = gettblPcode();
    table.makeCellVisible(table.rowAtPoint(mouseEvent.getPoint()),0);
}
/**
 * Called when process property changes
 * @param e event
 */
public void processChanged(ProcessChangeEvent e) {
    switch(e.getProperty()) {
        case ProcessChangeEvent.BOTTOM:
            gettxtBottom().setText(e.getNewValue().toString());
            break;
        case ProcessChangeEvent.TOP:
            gettxtTop().setText(e.getNewValue().toString());
            break;
        case ProcessChangeEvent.ACTIVE:
            gettxtActive().setText(e.getNewValue().toString());
            break;
        case ProcessChangeEvent.FINISHED:
            gettxtFinished().setText(e.getNewValue().toString());
            break;
        case ProcessChangeEvent.SUSPEND:
            gettxtSuspend().setText(e.getNewValue()==null?"":e.getNewValue().toString());
            break;
        case ProcessChangeEvent.MONITOR:
            gettxtMonitor().setText(e.getNewValue()==null?"":e.getNewValue().toString());
            break;
        case ProcessChangeEvent.PRIORITY:
            gettxtPriority().setText(e.getNewValue().toString());
            break;
        case ProcessChangeEvent.ATOMIC:
            gettxtAtomic().setText(e.getNewValue().toString());
            break;
    }
}
/**
 * Removes a pcode break point
 */
private void removePcodeBreakpoint() {
    PcodeModel model = (PcodeModel)gettblPcode().getModel();
    int selectedRow = gettblPcode().getSelectedRow();

    BreakpointManager manager = getDebugger().getInterpreter().getBreakpointManager();

    manager.removeBreakpoint(model.getBeginIndex()+selectedRow);
    gettblPcode().getSelectionModel().setSelectionInterval(selectedRow,selectedRow);
}
/**
 * Removes a source break point
 */
private void removeSourceBreakpoint() {
    SourceModel model = (SourceModel)gettblSource().getModel();
    int selectedRow = gettblSource().getSelectedRow();

    BreakpointManager manager = getDebugger().getInterpreter().getBreakpointManager();

    PcodeInstruction pcode = model.getProcess().getProgram().getPcode(model.getProcess().getCurrentPcode());
    manager.removeBreakpoint(pcode.getFileIndex(),selectedRow);
    gettblSource().getSelectionModel().setSelectionInterval(selectedRow,selectedRow);
}
/**
 * Called when mouse pressed on source table
 */
public void sourceTablePressed(MouseEvent mouseEvent) {
    CodeTable table = gettblSource();
    table.makeCellVisible(table.rowAtPoint(mouseEvent.getPoint()),0);
}
/** 
 * Called when selection in source table changes
 * @param e event
 */
public void valueChanged(ListSelectionEvent e) {
    ListSelectionModel source = (ListSelectionModel)e.getSource();
    int firstIndex = source.getMinSelectionIndex();

    if(firstIndex!=-1) {
        Program program = process.getProgram();
        int currentFile = program.getPcode(process.getCurrentPcode()).getFileIndex();

        PcodeModel model = process.getPcodeModel();
        model.setRange(program.getMappingFirst(currentFile,firstIndex),program.getMappingLast(currentFile,firstIndex));
    }
}
}
