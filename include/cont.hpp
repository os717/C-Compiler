#ifndef CONT_HPP
#define CONT_HPP

#include <string>
#include <unordered_map>

class CONT{
private:

public:
	int R1, R2, num_lab, siz_fra, num_paramet, num_sco;
	std::unordered_map<int, std::unordered_map<std::string, int> > GELS;
	CONT(std::unordered_map<int, std::unordered_map<std::string, int>> _GELS) : GELS(_GELS) {
		num_lab, num_paramet, num_sco = 0;
        R1, R2 = 2;
		siz_fra = 32;
	}
	std::string n_LAB(std::string NAM_labels){
  	return NAM_labels + std::to_string(num_lab++);
  }
	bool gl_CHK(std::string NAM){
  	int SCP = num_sco;
  	while(SCP > 0){
  		auto POSITION = GELS[SCP].find(NAM);
  		if(POSITION != GELS[SCP].end()){
  			return false;
  		}
  		SCP = SCP - 1;
  	}
  	return true;
  }
	int OfsFin(std::string NAM){
		int SCP = num_sco;
		while(SCP > 0){
			auto POSITION = GELS[SCP].find(NAM);
			if(POSITION != GELS[SCP].end()){
				return GELS[SCP][NAM];
			}
			SCP = SCP - 1;
		}
		return -1;
	}
	void R2Incr(){R2 = (R2 == 2) ? 3 : ((R2 == 3) ? 8 : R2 + 1);}
	void R2Decr(){R2 = (R2 == 3) ? 2 : ((R2 == 8) ? 3 : R2 - 1);}
};

#endif
