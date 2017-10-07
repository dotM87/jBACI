package baci.gui;

import baci.interpreter.*;
import javax.swing.*;
import javax.swing.table.*;
/**
 * Table cell render used for process tables
 * @author: Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class ProcessCellRenderer extends JTextField implements TableCellRenderer {

    private Interpreter interpreter;

public ProcessCellRenderer(Interpreter i) {
    interpreter = i;
    setEditable(false);
    setBorder(null);
    setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, Config.getIntegerProperty("TABLE_FONT_SIZE")));
}

/**
 * Called by JTable to get render component
 * @param table
 * @param value
 * @param isSelected
 * @param hasFocus
 * @param row
 * @param column
 */
public java.awt.Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
    if (hasFocus) table.repaint(); 
    if ((interpreter.getCurrentProcessIndex() == row)) {
        setForeground(table.getSelectionForeground());
        setBackground(table.getSelectionBackground());
    } else {
        setForeground(table.getForeground());
        setBackground(table.getBackground());
    }
    BaciProcess p = interpreter.getProcess(row);
    switch (column) {
        case 0: setText(p.getName()); break;
        case 1: setText(p.isActive() + ""); break;
        case 2:
	  String s;
	  if (p.getSuspend() == null) s = " ";
	  else if (p.getSuspend() == p.getMonitor()) s = p.getMonitorName();
	  else {	// Assume monitor is global, even in Pascal
	     VariableModel.VariableNode n = interpreter.getGlobalVariableModel().getNodeForStackIndex(p.getSuspend().getIndex());
             if (n == null) s = " "; else s = n.toString();
	  }
	  setText(s); 
	  break;
        case 3: setText(p.isFinished() + ""); break;
        case 4: setText(p.getMonitorName()==null ? " " : p.getMonitorName()); break;
        case 5: setText(p.getPriority() + ""); break;
        case 6: setText(p.getAtomicCount() + ""); break;
    }
    return this;
}
}
