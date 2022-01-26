from particle import Particle

def collide(particle1: Particle, particle2: Particle, dt) -> bool:
    # calc distance before
    delta_before = particle1.get_x() - particle2.get_x()
    # calc distance after dt
    delta_after = (particle1.get_x() + particle1.get_vx() * dt) - (particle2.get_x() + particle2.get_vx() * dt)
    # special case, strict equal
    if delta_before == 0 or delta_after == 0:
        return True
    else:
        # as we use the sign in delta_* if one is positiv and the other is negative, the particles collided
        return delta_after / delta_before < 0

def get_collision_time(particle1: Particle, particle2: Particle):
    # 0 = x + vx * dt - x2 + vx2 * dt
    # x2 - x = vx * dt + vx2 * dt
    # (x2 - x)/dt = vx - vx2
    # 1/dt = (vx- vx2) / (x2-x)
    # dt = (x2 - x) / (vx1 - vx2)
    dt = (particle2.get_x() - particle1.get_x()) / (particle1.get_vx() - particle2.get_vx())
    return dt

def move_to_collision_point(particle1: Particle, particle2: Particle) -> float:
    # cal collision time
    dt_collision = get_collision_time(particle1, particle2)

    # move to particules to the collision point
    particle1.move(dt_collision)
    particle2.move(dt_collision)

    # return td
    return dt_collision

def elastic_collision(particle1: Particle, particle2: Particle, dt) -> bool:
    # check has collision of exit
    if collide(particle1, particle2, dt) == False:
        return False

    # move to collision point
    dt_collision = move_to_collision_point(particle1, particle2)    

    # extract infos
    u1 = particle1.get_vx()
    m1 = particle1.get_mass()
    u2 = particle2.get_vx()
    m2 = particle2.get_mass()

    # apply elastic collision
    v1 = (u1* (m1 - m2) + 2 * m2 * u2) / (m1 + m2)
    v2 = (u2* (m2 - m1) + 2 * m1 * u1) / (m2 + m1)

    # apply back
    particle1.set_vx(v1)
    particle2.set_vx(v2)

    # move the rest of dt
    dt_rest = dt - dt_collision
    particle1.move(dt_rest)
    particle2.move(dt_rest)

    # return collided
    return True
