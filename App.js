import axios from 'axios';
import { StatusBar } from 'expo-status-bar';
import React, { useState } from 'react';
import { 
  StyleSheet,
  Text, 
  TouchableOpacity, 
  View 
} from 'react-native';

export default function App() {
  const [open, setOpen] = useState(true);
  const [waiting, setWaiting] = useState(false);
  
  const handleClick = async () => {
    setWaiting(true)
    await axios.get(`http://192.168.0.103/?pin9=${open?1:0}&pin10=${!open?1:0}}`, {timeout: 10000}).then(() => {
      setTimeout(() => {
        setWaiting(false);
        setOpen(!open);
      }, 2000);  
    }).catch(() => {
      setTimeout(() => {
        setWaiting(false);
      }, 2000);
    });
  };
  return (
    <>
      <StatusBar  style={'light'}/>
      <View style={styles.container}>
        
          <TouchableOpacity 
            style={open?styles.buttonOpen:styles.buttonClose} 
            onPress={handleClick}
            disabled={waiting}
          >
            <Text
              style={styles.buttonText}
            >
              {waiting?
                <>{open?'Abriendo...':'Cerrando...'}</>
                :
                <>{open?'Abrir!':'Cerrar!'}</>
              }
            </Text>
          </TouchableOpacity>
      </View>   
    </>
  );
}

const styles = StyleSheet.create({
  container: {
      flex: 1,
      backgroundColor: '#00172b',
      alignItems: 'center',
      justifyContent: 'center',
  },
  buttonText: {
    color: '#00172b',
    fontSize: 48,
  },
  buttonOpen: {
      width: '80%',
      maxHeight: '33.3%',
      borderRadius: 4,
      flex : 1,
      alignItems: 'center',
      justifyContent: 'center',
      backgroundColor: '#5cb85c'
  },
  buttonClose: {
      width: '80%',
      maxHeight: '33.3%',
      borderRadius: 4,
      flex : 1,
      alignItems: 'center',
      justifyContent: 'center',
      backgroundColor: '#d9534f'
  }
});
