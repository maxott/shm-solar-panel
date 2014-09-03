
public class SolarData {
	float n_nodes, p_nodes, p_sys, 
		v_bat, k_safety, time_charge, time_buf, 
		charge_efficiency, current_quiescent_mppt, 
		current_quiescent_pcb, charge_min, 
		buf_qiescent;
	float p_solar, battery_cap;
	boolean load_const;
	static float mppt_p_loss = 3.0f;
	
	public SolarData() {
		// TODO Auto-generated constructor stub
	}
	/**
	 * Updates the Data-Model with new data
	 * @param loat_const ... boolean flag weather load should be supplied constantly from solar power
	 * @param n_nodes ... quantity of nodes
	 * @param p_nodes ... power in watt per node
	 * @param p_sys ... power of the system
	 * @param v_bat ... battery voltage
	 * @param k_safety ... safety factor
	 * @param time_charge ... time to fully charge, at charging start battery assumed to be empty
	 * @param time_buf ... time to buffer the whole system from battery
	 * @param charge_efficiency ... charging efficiency of the battery
	 * @param current_quiescent_mppt ... quiescent current of mppt
	 * @param current_quiescent_pcb ... quiescent current of pcb
	 * @param charge_min ... remaining minimum capacity of battery after 
	 * @param buf_qiescent ... days to buffer quiescent current
	 */
	public void update(boolean loat_const, 
			float n_nodes, float p_nodes, 
			float p_sys, float v_bat, float k_safety, 
			float time_charge, float time_buf, 
			float charge_efficiency, float current_quiescent_mppt, 
			float current_quiescent_pcb, float charge_min, 
			float buf_qiescent){
		this.load_const = loat_const;
		this.n_nodes=n_nodes;
		this.p_nodes=p_nodes;
		this.p_sys = p_sys;
		this.v_bat = v_bat;
		this.k_safety = k_safety;
		this.time_charge = time_charge;
		this.time_buf = time_buf;
		this.charge_efficiency = charge_efficiency;
		this.current_quiescent_mppt = current_quiescent_mppt;
		this.current_quiescent_pcb = current_quiescent_pcb;
		this.charge_min = charge_min;
		this.buf_qiescent = buf_qiescent;
	}
	
	public float calculateSolarCells(){
		//Equation to calculate solar power
		//(power for nodes (if should be supplied by solar energy) and charging system within fully-charge-time + pcb quiescent current & mppt loss) * safety-factor
		p_solar = ((p_nodes*n_nodes + p_sys)*(load_const ? 1:0)  + calculateBatteryCap()/time_charge*v_bat/(charge_efficiency/100) + (current_quiescent_pcb/1000*v_bat)+mppt_p_loss)*k_safety;
		
		return p_solar;
	}
	public float calculateBatteryCap(){
		//Equation to calculate battery power
		//(quiescent current for x days + x hours to buffer nodes & system)*safety-factor * ensuring min remaining capacity 
		battery_cap = (buf_qiescent * 24 * (current_quiescent_mppt/1000 + current_quiescent_pcb/1000) + time_buf*(p_nodes*n_nodes+p_sys)/v_bat)*k_safety/(1-charge_min/100);
		return battery_cap;
	}
}
