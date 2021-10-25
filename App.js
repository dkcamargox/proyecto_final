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
  
  const handleClick = () => {
    setOpen(!open);
  };
  return (
    <>
      <StatusBar  style={'light'}/>
      <View style={styles.container}>
        
          <TouchableOpacity 
            style={open?styles.buttonOpen:styles.buttonClose} 
            onPress={handleClick}
          >
            <Text
              style={styles.buttonText}
            >
              {open?'Abrir!':'Cerrar!'}
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
    fontSize: 64,
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
