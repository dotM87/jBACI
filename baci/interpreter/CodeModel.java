package baci.interpreter;

/**
 * Super class of table models for source and pcode
 * @author: David Strite
 */
public abstract class CodeModel extends BaciTableModel {
/**
 * Creates a new CodeModel
 */
public CodeModel() {
}
/**
 * Gets the row number for the current pcode instruction
 * @return int
 */
public abstract int getCurrentRow();
}
