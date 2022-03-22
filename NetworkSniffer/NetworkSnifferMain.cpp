#include "NSFunctions.h"
#include "NSVarsAndStructs.h"
#include "NICOperations.h"


class Handle {
	public:
		NICOperations* y;

		void GetSelectFilterSave() {
			y->getNIC();
		}
};

int main() {
	Handle m;

	m.GetSelectFilterSave();

	return 0;
}