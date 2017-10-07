package baci.gui.actionbuttons;

import baci.gui.*;
import baci.interpreter.*;
import java.util.*;
import javax.swing.*;
/**
 * Super class of action buttons
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version December 2002
 * See copyright.txt
 */
public abstract class BaciActionButton extends AbstractAction {
    private Debugger debugger;
/**
 * Constructs a new BaciActionButton
 */
public BaciActionButton(int mn) {
    super();
    putValue(MNEMONIC_KEY, new Integer(mn));
    setEnabled(false);
}
/**
 * Constructs a new BaciActionButton with specified name
 * @param name
 */
public BaciActionButton(String name, int mn) {
    super(name);
    putValue(MNEMONIC_KEY, new Integer(mn));
    setEnabled(false);
}

/**
 * Constructs a new BaciActionButton with specified name
 * @param name
 */
public BaciActionButton(String name, int mn, String ac) {
    super(name);
    putValue(MNEMONIC_KEY, new Integer(mn));
    putValue(ACCELERATOR_KEY, javax.swing.KeyStroke.getKeyStroke(ac));
    setEnabled(false);
}

/**
 * Creates a new BaciActionButton with specified name and icon
 * @param name
 * @param icon
 */
public BaciActionButton(String name, Icon icon, int mn) {
    super(name, icon);
    putValue(MNEMONIC_KEY, new Integer(mn));
    setEnabled(false);
}
/**
 * Get the Debugger associated with this button
 * @return Debugger
 */
public Debugger getDebugger() {
    return debugger;
}
/**
 * Handles an exception by displaying message in DebuggerFrame
 * @param e
 */
public void handleException(Exception e) {
    getDebugger().getDebuggerFrame().showError(e);
}
/**
 * Sets the Debugger associated with this button
 * @param newDebugger
 */
public void setDebugger(Debugger newDebugger) {
    debugger = newDebugger;
}
}
