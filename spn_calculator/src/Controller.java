import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JOptionPane;
import javax.swing.JTextField;


public class Controller implements KeyListener, ActionListener{
	View v = new View(this);
	SolarData sd = new SolarData();
	public static void main(String[] args){
		// TODO Auto-generated method stub
		Controller c = new Controller();
		
		
	}
	public Controller() {
		// TODO Auto-generated constructor stub
		UpdateData();
		v.UpdateView(sd.calculateSolarCells(), sd.calculateBatteryCap());
	}
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
		UpdateData();
		//calculate
		v.UpdateView(sd.calculateSolarCells(), sd.calculateBatteryCap());
	}

	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub
		UpdateData();
		//calculate
		v.UpdateView(sd.calculateSolarCells(), sd.calculateBatteryCap());
	}

	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
		UpdateData();
		//calculate
	
		v.UpdateView(sd.calculateSolarCells(), sd.calculateBatteryCap());
	}

	public void actionPerformed(ActionEvent arg0) {
		// TODO Auto-generated method stub
		UpdateData();
		//calculate
		v.UpdateView(sd.calculateSolarCells(), sd.calculateBatteryCap());
	}
	
	private void UpdateData(){
		
		sd.update(v.inputfields.load_const.isSelected(), ConvertStringToFloat(v.inputfields.n_nodes), 
				ConvertStringToFloat(v.inputfields.p_nodes), ConvertStringToFloat(v.inputfields.p_sys) , 
				ConvertStringToFloat(v.inputfields.v_bat) , ConvertStringToFloat(v.inputfields.k_safety) ,
				ConvertStringToFloat(v.inputfields.time_charge) , ConvertStringToFloat(v.inputfields.time_buf) , 
				ConvertStringToFloat(v.inputfields.charge_efficiency) , ConvertStringToFloat(v.inputfields.current_quiescent_mppt) ,
				ConvertStringToFloat(v.inputfields.current_quiescent_pcb) , ConvertStringToFloat(v.inputfields.charge_min) , 
				ConvertStringToFloat(v.inputfields.buf_qiescent) );
	}
	/**
	 * Reads value from jTexfield-Object, converts to float and prints error message if text is invalid float value
	 * @param tf ... jTextfield which should be read from
	 * @return ... float read fom jTextfield
	 */
	private float ConvertStringToFloat(JTextField tf){
		float rv;
		if(tf.getText().length()!=0){
			try{
				rv=Float.parseFloat(tf.getText());
			}catch(Exception e){
				JOptionPane.showMessageDialog(v,
					    "Invalid Argument",
					    "Inane error",
					    JOptionPane.ERROR_MESSAGE);
				tf.setText("0");
				rv = 0;
			}
		}else{
			rv = 0;
		}
		return rv;
	}
}
