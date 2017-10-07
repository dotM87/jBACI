package baci.gui;

import javax.swing.*;
import javax.swing.table.*;
/**
 * Table cell render used for code tables
 * @author: David Strite
 * @author: Modified by Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */
public class CodeCellRenderer extends JTextField implements TableCellRenderer {
/**
 * Construct new CodeCellRenderer
 */

public CodeCellRenderer(boolean isSource) {
    setEditable(false);
    setBorder(null);
    if (isSource)
        setFont(new java.awt.Font(Config.SOURCE_FONT_FAMILY, Config.SOURCE_FONT_STYLE, 
           Config.getIntegerProperty("SOURCE_FONT_SIZE")));
    else
        setFont(new java.awt.Font(Config.PCODE_FONT_FAMILY, Config.PCODE_FONT_STYLE, 
	   Config.getIntegerProperty("PCODE_FONT_SIZE")));
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
    if (isSelected) {
        setForeground(table.getSelectionForeground());
        setBackground(table.getSelectionBackground());
    } else {
        setForeground(table.getForeground());
        setBackground(table.getBackground());
    }
    setText(value.toString());
    return this;
}
}
