from .entities.User import User


class ModelUser():

    @classmethod
    def login(self, db, user):
        try:
            cursor = db.connection.cursor()
            #Saber si el usuario existe en la base de datos 
            sql = """SELECT id, username, password, fullname FROM user 
                    WHERE username = '{}'""".format(user.username)
            cursor.execute(sql)
            row = cursor.fetchone()
            if row != None:
                #Hemos encontrado el usuario
                user = User(row[0], row[1], User.check_password(row[2], user.password), row[3])
                return user
            else:
                #si no hay retornamos no
                return None
        except Exception as ex:
            raise Exception(ex)

    @classmethod
    def get_by_id(self, db, id):
            try:
                cursor = db.connection.cursor()
                sql = "SELECT id, username, fullname FROM user WHERE id = {}".format(id)
                cursor.execute(sql)
                row = cursor.fetchone()
                if row != None:
                    return User(row[0], row[1], None, row[2])
                else:
                    return None
            except Exception as ex:
                raise Exception(ex)
class subirusuario():
    @staticmethod
    def sub(db, username, password, fullname, cedula, genero, edad):
        try:
            cursor=db.connection.cursor()
            cursor.execute("INSERT INTO user (username, password, fullname, cedula, genero, edad) VALUES (%s, %s, %s, %s, %s, %s)", (username, password, fullname, cedula, genero, edad))
            db.connection.commit()
            #return redirect(url_for)      
        except Exception as ex:
            raise Exception(ex)