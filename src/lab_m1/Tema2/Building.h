
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace implemented
{
    class Building
    {
    public:
        Building()
        {
        }

        ~Building()
        { }
        void SetBuilding(const glm::vec3 position) {
            this->position = position;
        }
        
       
        void UpdateMatrix() {
            glm::mat4 modelMatrix = glm::mat4(1);
            matrix = modelMatrix;
        }
    

    public:
        //matrice:
        glm::mat4 matrix = glm::mat4(1);
        glm::vec3 position;
        int damage = 0;
        int radius = 15;
    };

}

