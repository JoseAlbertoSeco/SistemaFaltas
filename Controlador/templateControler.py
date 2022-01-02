import datetime
import mysql.connector
import time
import json

class Conn:
    def __init__(self):
        self.conn = mysql.connector.connect(
            host="localhost",
            user="root",
            password="",
            database="sistemaFaltas"
        )
        self.cursor = self.conn.cursor()

def timeControler(now):
    print(now.hour)
    while 7 <= now.hour < 24:
        now = datetime.datetime.now()
        if now.hour == 7 and (50 < now.minute < 55):
            # 8:00
            fileControler(now.weekday(), 1)
            time.sleep(320)
        elif now.hour == 8 and (50 < now.minute < 55):
            # 9:00
            fileControler(now.weekday(), 2)
            time.sleep(320)
        elif now.hour == 9 and (50 < now.minute < 55):
            # 10:00
            fileControler(now.weekday(), 3)
            time.sleep(320)
        elif now.hour == 11 and (20 < now.minute < 25):
            # 11:30
            fileControler(now.weekday(), 4)
            time.sleep(320)
        elif now.hour == 12 and (50 < now.minute < 55):
            # 12:30
            fileControler(now.weekday(), 5)
            time.sleep(320)
        elif now.hour == 19 and (50 < now.minute < 57):
            # 13:30
            fileControler(now.weekday(), 6)
            time.sleep(320)
        time.sleep(60)


def fileControler(day, hour):

    ''' Obtiene el idCurso e idClase dónde se impartirá la clase en esa hora '''

    conn = Conn()
    query_h = "SELECT `H_idCurso`, `H_idAsigClase" + str(hour) + "` FROM `horario` WHERE `H_dia`=" + str(day)
    conn.cursor.execute(query_h)
    horario = conn.cursor.fetchall()

    ''' Por cada curso se obtienen los alumnos (por id y la huella) '''

    for h in horario:
        query_t = "SELECT `A_idAlumno`,`A_huellaTemplate` FROM `alumno` WHERE `A_idCurso`=" + str(h[0])
        conn.cursor.execute(query_t)
        templates = conn.cursor.fetchall()
        
        team = {}
        team['alumnos'] = []
        count = 0

        ''' Se crea un registro en faltas con:
                - idAlumno
                - hora de la clase
                - id de la asignatura (para saber también el aula)
                - F de falta en asistencia
            Se pone un F porque solo se modificarán los registros que estén en clase (P ó R) '''
			
        for t in templates:
            query_f = "INSERT INTO `faltas`(`F_idAlumno`, `F_hora`, `F_idAsignatura`, `F_asistencia`) VALUES (%s,%s,%s,%s)"
            val=(str(t[0]),str(hour),str(h[0]),"F")

            conn.cursor.execute(query_f, val)
            conn.conn.commit()

            print("REGISTRO AÑADIDO -> idAlumno: " + str(t[0]) + ", hora_clase: " + str(hour) + ", idCurso: " + str(h[0]))
            #print(conn.cursor.rowcount, "registro insertado")

            team['alumnos'].append({'id': str(t[0]),'huella': str(t[1])})
            count += 1

        team['count'] = str(count)

        with open('Templates/' + str(h[0]) + '.json', 'w') as f:
        	json.dump(team, f)

if __name__ == '__main__':
    fileControler(1,2)
