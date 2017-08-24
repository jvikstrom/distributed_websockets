import React, { Component } from 'react';
import { Container, Form, Button, Input } from 'semantic-ui-react';

export default class Login extends Component{
  /*
props:
ws (WebSocket), onLogin (Function)
  */
  constructor(props){
    super(props);
    this.state = {
      username: '',
      password: ''
    };
  }

  render(){
    return (
      <Container>
        <Form>
          <Form.Field>
            <label>Username</label>
            <Input placeholder='Username' onChange={(e) => this.setState({username: e.target.value})}/>
          </Form.Field>
          <Form.Field>
            <label>Password</label>
            <Input placeholder='Password' onChange={(e) => this.setState({password: e.target.value})}/>
          </Form.Field>
          <Button onClick={() => this.props.onLogin(this.state)}>Login</Button>
        </Form>
      </Container>
    )
  }
}
