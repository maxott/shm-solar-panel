import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionListener;
import java.awt.event.KeyListener;

import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;






public class View extends JFrame{
	ResultFields rf = new ResultFields();
	ExplainFields ef = new ExplainFields();
	InputFields inputfields;
	Controller cntrlr;
	JPanel jp = new JPanel();
	public View(Controller cntrlr) {
		// TODO Auto-generated constructor stub
		super("SPN - Dimensioning Solar Cell Power & Battery Capacity");
		
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setSize(500, 400);
		this.setLayout(new BorderLayout());
		this.cntrlr = cntrlr;
		
		
		inputfields = new InputFields(cntrlr, cntrlr);
		
		
		jp.setLayout(new GridLayout(1,2));
		jp.add(ef);
		jp.add(inputfields);
		this.add(jp, BorderLayout.CENTER);
		this.add(rf, BorderLayout.SOUTH);
		ef.repaint();
		inputfields.repaint();
		rf.repaint();
		this.repaint();
		this.setVisible(true);
	}
	/**
	 * Updates the view setting calculated solar power and battery capacity
	 * @param solar ... new Solar Power
	 * @param bat_cap ... new Battery Capacity
	 */
	public void UpdateView(float solar, float bat_cap){
		rf.battery_cap.setText("Battery Cap [Ah]: "+Float.toString(bat_cap));
		rf.solar_power.setText("Solar Cell Power Min [Watt]: "+Float.toString(solar));
	}
}
class ResultFields extends JPanel{
	JTextField solar_power, battery_cap;
	public ResultFields() {
		// TODO Auto-generated constructor stub
		this.setLayout(new GridLayout(1,2));
		this.setVisible(true);
		solar_power = new JTextField();
		battery_cap = new JTextField();
		this.add(solar_power);
		this.add(battery_cap);
		this.solar_power.setEditable(false);
		this.battery_cap.setEditable(false);
	}
}

class ExplainFields extends JPanel{
	JTextField load_const, n_nodes, p_nodes, p_sys, v_bat, k_safety, 
		time_charge, time_buf, charge_efficiency, current_quiescent_mppt, 
		current_quiescent_pcb, charge_min, buf_qiescent;
	
	public ExplainFields() {
		// TODO Auto-generated constructor stub
		this.setLayout(new GridLayout(13,1));
		n_nodes = new JTextField("How many nodes?");
		p_nodes = new JTextField("Power per Node [Watt]");
		p_sys = new JTextField("Power System [Watt]");
		v_bat = new JTextField("Battery-Voltage [V]");
		k_safety = new JTextField("Safety Factor");
		time_charge = new JTextField("Time to fully charge [h]");
		time_buf = new JTextField("Time to Buffer System from Battery [h]");
		charge_efficiency = new JTextField("Charging Efficiency [%]");
		current_quiescent_mppt = new JTextField("Quiescent Current MPPT [mA]");
		current_quiescent_pcb = new JTextField("Quiescent Current PCB [mA]");
		charge_min = new JTextField("Remaining Capacity Minimum [%]");
		buf_qiescent = new JTextField("Days to buffer Quiescent Current");
		load_const = new JTextField("Nodes directly from Solar?");
		
		this.add(n_nodes);
		this.add(p_nodes);
		this.add(p_sys);
		this.add(v_bat);
		this.add(k_safety);
		this.add(time_charge);
		this.add(time_buf);
		this.add(charge_efficiency);
		this.add(current_quiescent_mppt);
		this.add(current_quiescent_pcb);
		this.add(charge_min);
		this.add(buf_qiescent);
		this.add(load_const);
		
		n_nodes.setEditable(false);
		p_nodes.setEditable(false);
		p_sys.setEditable(false);
		v_bat.setEditable(false);
		k_safety.setEditable(false);
		time_charge.setEditable(false);
		time_buf.setEditable(false);
		charge_efficiency.setEditable(false);
		current_quiescent_mppt.setEditable(false);
		current_quiescent_pcb.setEditable(false);
		charge_min.setEditable(false);
		buf_qiescent.setEditable(false);
		load_const.setEditable(false);
	}
}
class InputFields extends JPanel{
	JTextField n_nodes, p_nodes, p_sys, v_bat, k_safety, time_charge, 
		time_buf, charge_efficiency, current_quiescent_mppt, 
		current_quiescent_pcb, charge_min, buf_qiescent;
	JCheckBox load_const;
	public InputFields(KeyListener kl, ActionListener al) {
		// TODO Auto-generated constructor stub
		this.setVisible(true);
		this.setLayout(new GridLayout(13,1));
		n_nodes = new JTextField("3");
		p_nodes = new JTextField("2.5");
		p_sys = new JTextField("5");
		v_bat = new JTextField("12");
		k_safety = new JTextField("1.3");
		time_charge = new JTextField("8");
		time_buf = new JTextField("2");
		charge_efficiency = new JTextField("90");
		current_quiescent_mppt = new JTextField("40");
		current_quiescent_pcb = new JTextField("5");
		charge_min = new JTextField("30");
		buf_qiescent = new JTextField("3");
		load_const = new JCheckBox();
		load_const.setSelected(true);
		
		this.add(n_nodes);
		this.add(p_nodes);
		this.add(p_sys);
		this.add(v_bat);
		this.add(k_safety);
		this.add(time_charge);
		this.add(time_buf);
		this.add(charge_efficiency);
		this.add(current_quiescent_mppt);
		this.add(current_quiescent_pcb);
		this.add(charge_min);
		this.add(buf_qiescent);
		this.add(load_const);
		
		n_nodes.addKeyListener(kl);
		p_nodes.addKeyListener(kl);
		p_sys.addKeyListener(kl);
		v_bat.addKeyListener(kl);
		k_safety.addKeyListener(kl);
		time_charge.addKeyListener(kl);
		time_buf.addKeyListener(kl);
		charge_efficiency.addKeyListener(kl);
		current_quiescent_mppt.addKeyListener(kl);
		current_quiescent_pcb.addKeyListener(kl);
		charge_min.addKeyListener(kl);
		buf_qiescent.addKeyListener(kl);
		load_const.addActionListener(al);
		
		
	}
}