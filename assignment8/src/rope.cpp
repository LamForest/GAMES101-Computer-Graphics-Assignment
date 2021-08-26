#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

#include <assert.h>

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        // const int num_mass = pinned_nodes.size();
        
        assert(num_nodes > 1);
        Vector2D interval = (end - start) / static_cast<double>(num_nodes - 1);

        std::cout << "start: " << start << ",  end:  " << end << std::endl;
        
        Vector2D pos = start;

        //构造质点
        for(int i = 0; i < num_nodes; ++i){
            this->masses.push_back(new Mass(pos, node_mass, true));
            
            std::cout << "point " << i << " : " << pos << std::endl;

            pos += interval;
        }
        for(int i = 1; i < num_nodes; ++i){
           masses[i]->pinned = false;
        }

        //构造弹簧
        for(int i = 0; i < num_nodes - 1; ++i){
            this->springs.push_back(new Spring(this->masses[i], this->masses[i+1], k));
        }
        
        assert(masses.size() == num_nodes);
        assert(springs.size() == num_nodes - 1);
        // assert ((pos - interval) == end);
//        Comment-in this part when you implement the constructor

    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Mass *start = s->m1;
            Mass *end = s->m2;

            double spring_cur_len = (end->position - start->position).norm();
            Vector2D unit_start_to_end = (end->position - start->position).unit();
            Vector2D force_on_start = 1 * (spring_cur_len - s->rest_length) * unit_start_to_end * (s->k == -1 ? 10000: s->k);

            Vector2D force_on_end = -1 * force_on_start;

            start->forces += force_on_start;
            
            // Vector2D start_damping_force = -k_damping * ((start->velocity - end->velocity) * (-unit_start_to_end)) * (-unit_start_to_end);
            // start->forces += start_damping_force;
            end->forces += force_on_end;
            // end->forces += -start_damping_force;

            // return 
             
        }

        const float k_damping = 0.013;
        for (int i = 0; i < masses.size() ;i++)
        {
            Mass *m = masses[i];
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity;
                
                

                // TODO (Part 2): Add global damping
                // 有点像是空气阻力，和课上讲的不太一样
                // It can be shown experimentally that in such cases the resistance to
                // motion is directly proportional to the velocity of the mass and,
                // naturally, opposes the motion.
                // https://faculty.washington.edu/seattle/physics227/reading/reading-3b.pdf
                m->forces += - k_damping * m->velocity; // += - k_damping * v(t)
                
                Vector2D a = m->forces / m->mass;
                // {
                //     //explicit Euler
                //     m->position = m->position + m->velocity * delta_t; // x(t+1) = x(t) + v(t) * dt;
                //     m->velocity = m->velocity + a * delta_t; // v(t+1) = v(t) + a(t) * dt
                //     printf("------------------------------\n");
                //     printf("%d-th m.position = (%.3f, %.3f), m.V = (%.3f, %.3f), m.a = (%.3f, %.3f), m.f = (%.3f, %.3f)\n",
                //     i,m->position.x, m->position.y, m->velocity.x, m->velocity.y,a.x, a.y,m->forces.x, m->forces.y
                //     );
                // }
                {
                    //semi-implicit Euler
                    m->velocity = m->velocity + a * delta_t; // v(t+1) = v(t) + a(t) * dt
                    m->position = m->position + m->velocity * delta_t; // x(t+1) = x(t) + v(t+1) * dt;
                    

                }
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Mass *start = s->m1;
            Mass *end = s->m2;

            double spring_cur_len = (end->position - start->position).norm();
            Vector2D unit_start_to_end = (end->position - start->position).unit();
            Vector2D force_on_start = 1 * (spring_cur_len - s->rest_length) * unit_start_to_end * s->k;

            Vector2D force_on_end = -1 * force_on_start;

            start->forces += force_on_start;
            end->forces += force_on_end;
        }

        const float k_damping = 0.00003;
        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += gravity;
                
                Vector2D a = m->forces / m->mass;

                Vector2D temp_position = m->position;
                
                Vector2D next_position = temp_position + (1-k_damping) * (temp_position - m->last_position) + delta_t * delta_t * a;

                m->last_position = m->position;
                m->position = next_position;

                // printf("------------------------------\n");
                // printf("?-th m.position = (%.3f, %.3f), m.V = (%.3f, %.3f), m.a = (%.3f, %.3f), m.f = (%.3f, %.3f)\n",
                // m->position.x, m->position.y, m->velocity.x, m->velocity.y,a.x, a.y,m->forces.x, m->forces.y
                // );
            

                // TODO (Part 3.1): Set the new position of the rope mass

                // TODO (Part 2): Add global damping
                // TODO (Part 4): Add global Verlet damping
            }
            m->forces = Vector2D(0, 0);
        }

        //Constraint
        for (auto &s : springs)
        {
            if(s->k == -1){
               
                Mass *start = s->m1;
                Mass *end = s->m2;

                double spring_cur_len = (end->position - start->position).norm();
                Vector2D unit_start_to_end = (end->position - start->position).unit();
                Vector2D offset = (spring_cur_len - s->rest_length) * 0.5 * unit_start_to_end;

                Vector2D end_offset;
                Vector2D start_offset;
                if(!start->pinned || !end->pinned){
                    if(start->pinned){
                        end_offset = -2 * offset;
                    }
                    else if(end->pinned){
                        start_offset = 2 * offset;
                    }
                    else{
                        end_offset = -1 * offset;
                        start_offset = offset; 
                    }
                }
                start->position += start_offset;
                end->position += end_offset;
            }
            
        }

    }
}
