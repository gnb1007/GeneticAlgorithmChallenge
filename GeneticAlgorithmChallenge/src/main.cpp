#include <iostream>
#include <string>
#include <vector>
#include <random>

int mutations_per_thousand_genes = 75;

std::vector<std::vector<int>> travel_weight = {
	{0, 10, 153, 42, 37, 920, 410, 13},
	{10, 0, 8, 27, 93, 45, 21, 18},
	{153, 8, 0, 3, 21, 97, 410, 38},
	{42, 27, 3, 0, 22, 45, 81, 6},
	{37, 93, 21, 22, 0, 19, 80, 13},
	{920, 45, 97, 45, 19, 0, 18, 23},
	{410, 21, 410, 81, 80, 18, 0, 5},
	{13, 18, 38, 6, 13, 23, 5, 0}
};

std::string RandomGene() {
	return std::string(1, (char)(65 + rand() % 8));
}

int GeneToIndex(char c) {
	return c - 65;
}

struct Species {
	std::vector<std::string> chromossomes;
	Species() {
		for(int i = 0; i < 5; i++) {
			int chromossome_size = 2 + rand() % 9;
			std::string chromossome = "";
			for(int j = 0; j < chromossome_size; j++) {
				chromossome.append(RandomGene());
			}
			chromossomes.push_back(chromossome);
		}
	}

	Species(Species a, Species b) {
		for(int i = 0; i < 5; i++) {
			if(rand() % 2) {
				for(int j = 0; j < a.chromossomes[i].size(); j++) {
					if(1 + rand() % 1000 < mutations_per_thousand_genes) {
						a.chromossomes[i][j] = (char)(65 + rand() % 8);
					}
				}
				this->chromossomes.push_back(a.chromossomes[i]);
			} else {
				for(int j = 0; j < b.chromossomes[i].size(); j++) {
					if(1 + rand() % 1000 < mutations_per_thousand_genes) {
						b.chromossomes[i][j] = (char)(65 + rand() % 8);
					}
				}
				this->chromossomes.push_back(b.chromossomes[i]);
			}
		}
	}

	int GetFitness() {
		std::string genetic_code = "";
		for(int i = 0; i < 5; i++) {
			genetic_code.append(chromossomes[i]);
		}
		for(int i = 66; i <= 72; i++) {
			bool contains_i = false;
			for(int j = 0; j < genetic_code.size(); j++) {
				if(genetic_code[j] == i) {
					contains_i = true;
					break;
				}
			}
			if(!contains_i) {
				return 1e9;
			}
		}
		int total_travel_cost = 0;
		char last_gene = 'A';
		for(int i = 0; i < 5; i++) {
			std::string chromossome = chromossomes[i];
			while(!chromossome.empty()) {
				char current_gene = chromossome.back();
				chromossome.pop_back();
				total_travel_cost += travel_weight[GeneToIndex(current_gene)][GeneToIndex(last_gene)];
				last_gene = current_gene;
			}
		}
		total_travel_cost += travel_weight[GeneToIndex(last_gene)][GeneToIndex('A')];
		return total_travel_cost;
	}

};

bool IsFitter(Species a, Species b) {
	return a.GetFitness() < b.GetFitness();
}

int main(int argc, char** argv) {
	int population_size = 100;
	int population_seed_size = 10;
	int num_of_generations = 1000;
	std::vector<Species> population = std::vector<Species>(population_size);
	for(int i = 0; i < num_of_generations; i++) {
		std::vector<Species> top;
		for(int j = 0; j < population_seed_size; j++) {
			top.push_back(population[j]);
		}
		std::sort(top.begin(), top.end(), IsFitter);
		for(int j = population_seed_size; j < population_size; j++) {
			int new_pos = -1;
			for(int k = 0; k < population_seed_size; k++) {
				if(IsFitter(population[j], top[population_seed_size - 1 - k])) {
					new_pos = population_seed_size - 1 - k;
				} else {
					break;
				}
			}
			if(new_pos > 0) {
				for(int k = population_seed_size - 1; k >= new_pos; k--) {
					top[k] = top[k - 1];
				}
				top[new_pos] = population[j];
			}
		}
		population.clear();
		for(int j = 0; j < population_seed_size; j++) {
			for(int k = 0; k < population_seed_size; k++) {
				population.push_back(Species(top[j], top[k]));
			}
		}
	}
	std::sort(population.begin(), population.end(), IsFitter);
	for(int i = 0; i < 100; i++) {
		std::cout << population[i].GetFitness() << std::endl;
	}
	for(int i = 0; i < 5; i++) {
		std::reverse(population[0].chromossomes[i].begin(), population[0].chromossomes[i].end());
	}
	std::cout << "A" << population[0].chromossomes[0] << population[0].chromossomes[1] << population[0].chromossomes[2] << population[0].chromossomes[3] << population[0].chromossomes[4] << "A";
}